// tcp_tracer.cpp
#include "tcp_tracer.h"
#include "parse_value.h"
#include <cstdio>
#include <cstdlib>

config_t parse_config(char *config_str) {
    if (config_str == nullptr) {
        fprintf(stderr, "Invalid configuration string\n");
        exit(EXIT_FAILURE);
    }

    config_t config;
    char *ptr = config_str;
    char *endptr;

    // Parse port
    if (parse_value(ptr, &endptr, &config.port) != 0) {
        fprintf(stderr, "Failed to parse port\n");
        exit(EXIT_FAILURE);
    }

    // Move past the comma and space
    ptr = endptr + 2;

    // Parse storage port
    if (parse_value(ptr, &endptr, &config.storage_port) != 0) {
        fprintf(stderr, "Failed to parse storage port\n");
        exit(EXIT_FAILURE);
    }

    return config;
}
