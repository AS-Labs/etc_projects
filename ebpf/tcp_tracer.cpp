#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#include <bpf/bpf.h>
#include <bpf/libbpf.h>

#include <curl/curl.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <ctype.h>

#include "parse_value.h"


#define CONFIG_CONTENT \
    "host: 192.168.1.1\n" \
    "remote: 8.8.8.8\n" \
    "port: 443\n" \
    "format: loki\n" \
    "\n" \
    "remote_storage:\n" \
    "  host: https://localhost:3100\n" \
    "  port: 3100\n" \
    "  ssl: true\n"

// Struct for config file parsing

typedef struct {
    char host[64];
    char remote[64];
    uint16_t port;
    char format[8]; // Either "loki" or "prometheus"
    char storage_host[128];
    uint16_t storage_port;
    bool ssl;
} config_t;


static const char *SECTION = "tcp_tracer";


static void skip_whitespace(char **ptr) {
    while (**ptr && isspace(**ptr)) {
        (*ptr)++;
    }
}

int parse_value(char *str, char **endptr, uint16_t *value_ptr);

static int parse_value(char **prt, char *str, char **endptr, uint16_t *value_ptr) {
    char *ptr = str;
    skip_whitespace(&ptr);

    if (*ptr == '\0') {
        return -1;
    }

    while (isdigit(*ptr)) {
        ptr++;
    }
    
    if (*ptr != '\0') {
        return -1;
    }

    *value_ptr = atoi(str);
    *endptr = ptr;

    return 0;
}

static int parse_bool(char *str, char **endptr, bool *bool_ptr) {
    char *ptr = str;
    skip_whitespace(&ptr);

    if (strcmp(ptr, "true") == 0) {
        *bool_ptr = true;
        *endptr = ptr + strlen("true");
        return 0;
    } else if (strcmp(ptr, "false") == 0) {
        *bool_ptr = false;
        *endptr = ptr + strlen("false");
        return 0;
    }

    return -1;
}

static int parse_string(char *str, char **endptr, char *out_str, size_t out_size) {
    char *ptr = str;
    skip_whitespace(&ptr);

    if (*ptr != '\0') {
        size_t len = strlen(ptr);

        if (len >= out_size - 1) {
            return -1;
        }

        strncpy(out_str, ptr, len);
        out_str[len] = '\0';
        *endptr = ptr + len;

        return 0;

    }

    return -1;
}


// Function to parse the file
config_t parse_config(const char *file) {
    config_t config = {0};
    char *ptr = (char *)CONFIG_CONTENT;
    char *endptr;

    while (*ptr != '\0') {
        skip_whitespace(&ptr);

        if (*ptr == '#' || *ptr == '\n') {
            ptr++;
            continue;
        }

        if (sscanf(ptr, "host: %63[^\n]\n", config.host) != 1 ||
            sscanf(ptr, "remote: %63[^\n]\n", config.remote) != 1 ||
            parse_value(ptr, &endptr, &config.port) == -1 ||
            parse_string(ptr, &endptr, config.format, sizeof(config.format)) == -1) {

            fprintf(stderr, "Invalid configuration: %s\n", ptr);
            return (config_t){0};
        }

        if (*endptr != '\n') {
            fprintf(stderr, "Missing newline after key-value pair\n");
            return (config_t){0};
        }
        ptr = endptr + 1;

    }

    if (parse_string(ptr, &endptr, config.storage_host, sizeof(config.storage_host)) == -1 ||
        parse_value(endptr, &endptr, &config.storage_port) == -1 ||
        parse_bool(endptr, &endptr, &config.ssl) == -1) {
    fprintf(stderr, "Invalid configuration: %s\n", ptr);
        return (config_t){0};
    }
    if (*endptr != '\0') {
        fprintf(stderr, "Trailing garbage after remote_storage");
        return (config_t){0};
    }

    return config;

    
}



int main() {
    config_t config = parse_config(CONFIG_CONTENT);

    if (strcmp(config.format, "loki") == 0) {
        printf("Using loki with host %s and remote %s:%u\n", config.host, config.remote, config.port);
    } else if (strcmp(config.format, "prometheus") == 0) {
        printf("Using prometheus with host %s and remote %s:%u\n", config.host, config.remote, config.port);
    } else {
        fprintf(stderr, "Unknown format: %s\n", config.format);
        return EXIT_FAILURE;
    }

    if (config.storage_port != 0) {
        printf("Storing data at %s:%u with SSL %s\n", config.storage_host, config.storage_port, config.ssl ? "enabled" : "disabled");
    }

    return EXIT_SUCCESS;

}











