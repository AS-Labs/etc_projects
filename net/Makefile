CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I/usr/local/include
LDFLAGS = -L/usr/local/lib -luv

PROXY_SOURCE = tcp-proxy.c
PROXY_EXECUTABLE = tcp-proxy


tcp-proxy: $(PROXY_EXECUTABLE)

$(PROXY_EXECUTABLE): $(PROXY_SOURCE)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROXY_EXECUTABLE) $(PROXY_SOURCE)

clean:
	rm -f $(PROXY_EXECUTABLE)
