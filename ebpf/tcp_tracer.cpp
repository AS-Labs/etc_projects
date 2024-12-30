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
        size_t len = 0;

        while (&ptr && !isspace(&ptr)) {
            len++;
            ptr++;
        }
    }
}


// Function to parse the file
config_t parse_config(const char *file) {
    config_t config;
    
}











