#include <iostream>
#include <vector>
#include <unistd.h>
#include <csignal>
#include "../include/Analyzer.h"
#define BUFFER_SIZE 16384  // 16KB

volatile std::sig_atomic_t stop = 0;

void handle_sigint(int signum) {
    stop = 1;
}

int main() {
    std::vector<uint8_t> buffer(BUFFER_SIZE);
    ssize_t len;
    Analyzer analyzer;
    // Setup signal handler for SIGINT (Ctrl+C)
    std::signal(SIGINT, handle_sigint);

    while (!stop && (len = read(STDIN_FILENO, buffer.data(), BUFFER_SIZE)) > 0) {
        analyzer.analyze(buffer);  // Call analyze from Analyzer class
    }

    if (len < 0) {
        perror("read");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
