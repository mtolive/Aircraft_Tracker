#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include <bitset>

class Decoder{

private: 

    std::string binValues;   
    std::vector<std::string> binValuesGroup6;
    std::vector<std::string> decValues;


public:
    
    std::string hexToBinary(const std::string& hexValue);
    std::vector<std::string> group6Binary(const std::string& binValues);
    std::vector<std::string> binaryToDecimal(const std::vector<std::string>& binValues);

};