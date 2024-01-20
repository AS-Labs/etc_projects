#include <stdio.h>
#include <stdlib.h>
#include <uv.h>


#define CLIENT_PORT 7999
#define REMOTE_SERVER_IP "192.168.0.188"
#define REMOTE_SERVER_PORT 443


typedef struct {
	uv_tcp_t client;
	uv_tcp_t remote;
	uv_write_t write_req;
	char buffer[4096];
} proxy_t;

// closing the connection is basically freeing up the allocated memory for the socket.
void on_close(uv_handle_t* handle) {
	free(handle);
}

// Function to read from the remote system
void on_remote_read(uv_stream_t* remote, ssize_t nread, const uv_buf_t* buf) {
	if (nread > 0) {
		proxy_t* proxy = (proxy_t*)remote->data;
		uv_write(&proxy->write_req, (uv_stream_t*)&proxy->client, buf, 1, NULL);
	} else {
		if (nread < 0) {
			proxy_t* proxy = (proxy_t*)remote->data;
			uv_close((uv_handle_t*)remote, on_close);
			uv_close((uv_handle_t*)&proxy->client, on_close);
		}
		free(buf->base);
	}
}

void on_client_read(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) {
	if (nread > 0) {
		proxy_t* proxy = (proxy_t*)client->data;
		uv_write(&proxy->write_req, (uv_stream_t*)&proxy->remote, buf, 1, NULL);
	} else {
		if (nread < 0) {
			proxy_t* proxy = (proxy_t*)client->data;
			uv_close((uv_handle_t*)client, on_close);
			uv_close((uv_handle_t*)&proxy->remote, on_close);
		}
		free(buf->base);
	}
}


void on_connection(uv_stream_t* server, int status) {
	if (status == -1){
		fprintf(stderr, "Connection error\n");
		return;
	}

	proxy_t* proxy = (proxy_t*)malloc(sizeof(proxy_t));

	uv_tcp_init(server->loop, &proxy->client);
	uv_tcp_init(server->loop, &proxy->remote);

	proxy->client.data = proxy;
	proxy->remote.data = proxy;

	uv_accept(server, (uv_stream_t*)&proxy->client);

	struct sockaddr_in remote_addr;
	uv_ip4_addr(REMOTE_SERVER_IP, REMOTE_SERVER_PORT, &remote_addr);


	uv_read_start((uv_stream_t*)&proxy->client, (uv_alloc_cb)uv_buf_alloc, on_client_read);
	uv_read_start((uv_stream_t*)&proxy->remote, (uv_alloc_cb)uv_buf_alloc, on_remote_read);

}

int main() {
	uv_loop_t* loop = uv_default_loop();

	uv_tco_t server;
	uv_tcp_init(loop, &server);

	struct sockaddr_in bind_addr;
	uv_ip4_addr("0.0.0.0" CLIENT_PORT, &bind_addr);

	uv_tcp_bind(&server, (struct sockaddr*)&bind_addr, 0);
	int result = uv_listen((uv_stream_t*)&server, 128, on_connection);

	if (result) {
		fprintf(stderr, "Listen port issue: %s\n", uv_strerror(result));
		return 1;
	}


	printf("Proxy started on port %d ..\n", CLIENT_PORT);

	return uv_run(loop, UV_RUN_DEFAULT);
}
