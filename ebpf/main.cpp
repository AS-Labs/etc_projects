// main.cpp
#include "tcp_tracer.h"
#include <iostream>

int main() {
    const char *config_str = "1234, 5678";
    config_t config = parse_config(config_str);

    std::cout << "Port: " << config.port << std::endl;
    std::cout << "Storage Port: " << config.storage_port << std::endl;

    return 0;
}
