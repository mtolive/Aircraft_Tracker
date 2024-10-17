#include <iostream>
#include <vector>
#include <unistd.h>
#include <string>
#include <csignal>
#include <algorithm> // Include this header for std::remove
#include "../include/Analyzer.h"
#include "../include/Decoder.h"
#include "../include/Filter.h"
#include "../include/ADSBFrame.h"

#define BUFFER_SIZE 16384  // 16KB

volatile std::sig_atomic_t stop = 0;

void handle_sigint(int signum) {
    stop = 1;
}

int main() {
    std::vector<uint8_t> buffer(BUFFER_SIZE);
    ssize_t len;

    Decoder decoder;
    Filter filter(decoder); // Create an instance of Filter
    ADSBFrame frame(filter);  
    Analyzer analyzer(frame); // Pass the filter instance to Analyzer

    // Setup signal handler for SIGINT (Ctrl+C)
    std::signal(SIGINT, handle_sigint);

    std::string hexString;

    // Read from standard input (pipe from rtl_adsb)
    while (std::getline(std::cin, hexString)) {

        // Trim any whitespace characters and format
        hexString = analyzer.trim(hexString);
        // Convert the hex string to a byte vector
        std::vector<uint8_t> byteVector = analyzer.hexStringToVector(hexString);
        if (!byteVector.empty()) {
            // Analyze the hex string
            analyzer.analyze(byteVector);
        } else {
            std::cerr << "Failed to parse hex string!" << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
