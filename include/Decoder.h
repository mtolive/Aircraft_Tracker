#ifndef DECODER_H
#define DECODER_H

#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include <bitset>
#include <unordered_map>

class Decoder{

private: 

    std::string callSign;
    std::string binValues;   
    std::vector<std::string> binValuesGroup6;
    std::vector<std::string> decValues;
    std::unordered_map<int,char> asciiValues;


public:
    Decoder(){
        initializeMap();
    };
    void initializeMap();
    const std::string& hexToBinary(const std::string& hexValue);
    const std::vector<std::string>& group6Binary(const std::string& binValues);
    const std::vector<std::string>& binaryToDecimal(const std::vector<std::string>& binValuesGroup6);
    void hexToDecimal(const std::string& hexValues); // group 6 binary
    const std::string& decimalToAlpha(const std::vector<std::string>& decValues);
    void printDecValues() const;
};

#endif