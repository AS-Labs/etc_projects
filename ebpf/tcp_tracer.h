// tcp_tracer.h
#ifndef TCP_TRACER_H
#define TCP_TRACER_H

#include <cstddef>

struct config_t {
    unsigned short port;
    unsigned short storage_port;
};

config_t parse_config(const char *config_str);

#endif // TCP_TRACER_H
