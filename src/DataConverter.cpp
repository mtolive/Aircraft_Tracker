#include "../include/DataConverter.h"

void DataConverter::initializeMap() {
    asciiValues = {
        {1, 'A'}, {2, 'B'}, {3, 'C'}, {4, 'D'}, {5, 'E'}, {6, 'F'}, {7, 'G'}, {8, 'H'}, {9, 'I'}, {10, 'J'},
        {11, 'K'}, {12, 'L'}, {13, 'M'}, {14, 'N'}, {15, 'O'}, {16, 'P'}, {17, 'Q'}, {18, 'R'}, {19, 'S'}, {20, 'T'},
        {21, 'U'}, {22, 'V'}, {23, 'W'}, {24, 'X'}, {25, 'Y'}, {26, 'Z'}, {32, '_'}, {48, '0'}, {49, '1'}, {50, '2'},
        {51, '3'}, {52, '4'}, {53, '5'}, {54, '6'}, {55, '7'}, {56, '8'}, {57, '9'}
    };
}

std::string DataConverter::binaryToHex(const std::vector<uint8_t>& message) {
    std::ostringstream str;
    for (const auto& byte : message) {
        str << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return str.str();
}

std::string DataConverter::hexToBinary(const std::string& hexValues) {
    std::string binary;
    for (char c : hexValues) {
        binary += std::bitset<4>(std::stoi(std::string(1, c), nullptr, 16)).to_string();
    }
    return binary;
}

std::vector<std::string> DataConverter::binaryToDecimal(const std::vector<std::string>& binValues) {
    std::vector<std::string> decimalValues;
    for (const auto& bin : binValues) {
        decimalValues.push_back(std::to_string(std::stoul(bin, nullptr, 2)));
    }
    return decimalValues;
}

int DataConverter::hexToDecimal(const std::vector<uint8_t>& message){
    int decimalValue = 0;
    for(auto byte : message){
        decimalValue = (decimalValue << 8) | byte;
    }
    return decimalValue;
}

/***************************************************************************
*********************** Conversions For Callsign ***************************
***************************************************************************/
std::vector<std::string> DataConverter::group6Binary(const std::string& binValues) {
    std::vector<std::string> result;
    for (size_t i = 0; i < binValues.length(); i += 6) {
        result.push_back(binValues.substr(i, 6));
    }
    return result;
}

std::vector<std::string> DataConverter::group6Binary(const std::vector<uint8_t>& binary) {
    std::string binaryString;
    for (const auto& byte : binary) {
        binaryString += std::bitset<8>(byte).to_string();
    }
    return group6Binary(binaryString);  // Reuse the other group6Binary method
}

std::string DataConverter::decimalToAlpha(const std::vector<std::string>& decValues) {
    std::string result;
    for (const auto& dec : decValues) {
        int key = std::stoi(dec);
        result += asciiValues.count(key) ? asciiValues[key] : '\0';
    }
    return result;
}

std::string DataConverter::binToAlpha(const std::vector<uint8_t>& binary) {
    auto binGroups = group6Binary(binary);
    auto decValues = binaryToDecimal(binGroups);
    return decimalToAlpha(decValues);
}

std::string DataConverter::hexToAlpha(const std::string& hexValues) {
    auto binary = hexToBinary(hexValues);
    auto binGroups = group6Binary(binary);
    auto decValues = binaryToDecimal(binGroups);
    return decimalToAlpha(decValues);
}

const std::string& DataConverter::getCallSign() const {
    return callSign;
}