// parse_value.cpp
#include "parse_value.h"
#include <cstdlib>
#include <cstring>

int parse_value(char *str, char **endptr, unsigned short *value_ptr) {
    if (str == nullptr || endptr == nullptr || value_ptr == nullptr) {
        return -1; // Invalid arguments
    }

    *value_ptr = static_cast<unsigned short>(strtoul(str, endptr, 0));
    if (**endptr != '\0') {
        return -1; // Conversion error or non-numeric characters present
    }

    return 0; // Success
}
