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

//void on_client_read()
