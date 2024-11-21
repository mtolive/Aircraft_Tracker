#ifndef EXTRACTBITS_H
#define EXTRACTBITS_H

#include <iostream>
#include <vector>
#include <bitset>

class ExtractBits{
public:
    
    std::vector<uint8_t> getBitRange(size_t lowBit, size_t highBit, const std::vector<uint8_t>& binVector);
    std::vector<uint8_t> getBitRange(size_t lowBit, size_t highBit, const std::string& binStr);
private:
    std::vector<uint8_t> stov(const std::string& binStr);
    bool isBinary(const std::string& str);
};

#endif