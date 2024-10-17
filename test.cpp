#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>  // for std::transform

// Trim leading and trailing whitespace
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first == std::string::npos) ? "" : str.substr(first, last - first + 1);
}

// Converts hex string to vector of bytes
std::vector<uint8_t> strToBin(const std::string& hex) {
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        uint8_t byte = (uint8_t)strtol(byteString.c_str(), nullptr, 16);
        bytes.push_back(byte);
    }
    return bytes;
}

// Print hex values from a vector of bytes
void print_hex(const std::vector<uint8_t>& buffer) {
    for (size_t i = 0; i < buffer.size(); i++) {
        std::cout << std::hex << std::uppercase << std::setw(2) 
                  << std::setfill('0') << static_cast<int>(buffer[i]);
    }
    std::cout << std::endl;
}

// Function to convert string to uppercase
std::string toUpper(const std::string& str) {
    std::string upperStr = str;
    std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
    return upperStr;
}

int main() {
    std::string line;

    // Read from standard input
    while (std::getline(std::cin, line)) {

        // Trim leading/trailing whitespace or newlines
        line = trim(line);

        // Convert the input line to uppercase
        std::string upperLine = toUpper(line);

        // Check if the line has the correct format
        if (upperLine.size() == 30 && upperLine.front() == '*' && upperLine.back() == ';') {

            std::string hex_line = upperLine.substr(1, 28);  // Remove '*' and ';'

            // Convert hex string to bytes
            std::vector<uint8_t> hex = strToBin(hex_line);

             // extract downlink format (DF) from the first byte
            uint8_t df = (hex[0] >> 3) & 0x1F; // first 5 bits
            // extract type code (TC) from byte 5
            uint8_t tc = (hex[4] >> 3) & 0x1F; // bits 33-37   

            // Print the byte representation in hex format
            // && ( tc == 4 || tc == 3 || tc == 2 || tc == 1)
            // && tc == 4
            if(df == 17 && ( tc == 4 || tc == 3 || tc == 2 || tc == 1))
                print_hex(hex);
        } else {
            // Debug: show when input doesn't match the expected format
            std::cout << "Invalid input format or size" << std::endl;
        }
    }
}
