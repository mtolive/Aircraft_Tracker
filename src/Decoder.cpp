#include "../include/Decoder.h"

std::string Decoder::hexToBinary(const std::string& hexValues){
    binValues.clear();

    std::string binary;
    for (char c : hexValues) {
        // Convert each hex digit to a 6-bit binary representation
        binary += std::bitset<4>(std::stoi(std::string(1, c), nullptr, 16)).to_string();
    }
    return binary;
}

std::vector<std::string> Decoder::group6Binary(const std::string& binValues){
    binValuesGroup6.clear();

    size_t size = binValues.length();
    for(size_t i = 0; i < size; i += 6){
        std::string str = binValues.substr(i,6); // 6 bits at a time
        binValuesGroup6.push_back(str);
    }
    return binValuesGroup6;
}

std::vector<std::string> Decoder::binaryToDecimal(const std::vector<std::string>& binValues){

    size_t len = binValues.size();

    for(std::string bin : binValues){

        std::string str = std::to_string(std::stoul(bin, nullptr, 2));
        decValues.push_back(str);
    }
    return decValues;
}