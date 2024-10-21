#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>  // for std::transform
#include <unordered_map> // for std::unordered_map
#include <unordered_set>
#include <set> // for std::set

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

// Structure to store ADS-B message data for each ICAO
struct ADSBData {
    std::string callsign;
    std::string position; // Or any other fields you want to track
    std::string velocity;
};

// Create a map for ICAO to ADSBData
std::unordered_map<std::string, ADSBData> icaoData;
std::unordered_set<std::string> storedIcaos; // Set of stored ICAO addresses
const size_t maxStoredMessages = 1000; // Set max limit for stored messages

void processMessage(int df, int tc, const std::string& icao, const std::string& hex) {
    if (df == 17 && (tc >= 1 && tc <= 4)) {
        // Store initial messages up to the max limit
        if (storedIcaos.size() < maxStoredMessages) {
            std::cout << "Initial Message (Callsign): ";
            std::vector<uint8_t> hexBytes = strToBin(hex);
            print_hex(hexBytes);
            icaoData[icao].callsign = hex; // Store callsign by ICAO address
            storedIcaos.insert(icao);      // Mark this ICAO address as stored
        }
    } 
    else if (df == 17 && (tc >= 9 && tc <= 22)) {
        // Check if the ICAO address matches one we already processed
        if (icaoData.find(icao) != icaoData.end()) {
            std::cout << "Related Message: ";
            std::vector<uint8_t> hexBytes = strToBin(hex);
            print_hex(hexBytes);
            // Update position, velocity, or other relevant data here
            // Example: icaoData[icao].position = some_position_value;
        }
    }
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

            // Extract downlink format (DF) from the first byte
            uint8_t df = (hex[0] >> 3) & 0x1F; // first 5 bits
            // Extract ICAO address (first 6 hex characters)
            std::string icao = hex_line.substr(2, 6);

            // Extract type code (TC) from byte 5
            uint8_t tc = (hex[4] >> 3) & 0x1F; // bits 33-37   

            // Process the message
            processMessage(df, tc, icao, hex_line);
        } else {
            // Debug: show when input doesn't match the expected format
            std::cout << "Invalid input format or size" << std::endl;
        }
    }
}
