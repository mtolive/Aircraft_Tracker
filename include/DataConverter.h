#ifndef DATACONVERTER_H
#define DATACONVERTER_H

#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include <bitset>
#include <unordered_map>
#include <optional>

class DataConverter {

private:
    std::unordered_map<int, char> asciiValues;
    std::string callSign;

public:
    DataConverter() {
        initializeMap();
    };

    void initializeMap();
    
    std::string binaryToHex(const std::vector<uint8_t>& message);
    std::string hexToBinary(const std::string& hexValue);
    std::vector<std::string> binaryToDecimal(const std::vector<std::string>& binValues);
    int hexToDecimal(const std::vector<uint8_t>& message);
    /**************************************** 
     ************ Callsign logic ************
     ***************************************/
    std::vector<std::string> group6Binary(const std::string& binValues);
    std::vector<std::string> group6Binary(const std::vector<uint8_t>& binary);
    std::string decimalToAlpha(const std::vector<std::string>& decValues);
    std::string binToAlpha(const std::vector<uint8_t>& binary);
    std::string hexToAlpha(const std::string& hexValues);
    const std::string& getCallSign() const;
};

#endif
