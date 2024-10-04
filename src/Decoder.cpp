#include "../include/Decoder.h"

void Decoder::initializeMap(){
    asciiValues[1] = 'A'; asciiValues[2] = 'B'; asciiValues[3] = 'C'; asciiValues[4] = 'D'; asciiValues[5] = 'E';
    asciiValues[6] = 'F'; asciiValues[7] = 'G'; asciiValues[8] = 'H'; asciiValues[9] = 'I'; asciiValues[10] = 'J';
    asciiValues[11] = 'K'; asciiValues[12] = 'L'; asciiValues[13] = 'M'; asciiValues[14] = 'N'; asciiValues[15] = 'O';
    asciiValues[16] = 'P'; asciiValues[17] = 'Q'; asciiValues[18] = 'R'; asciiValues[19] = 'S'; asciiValues[20] = 'T';
    asciiValues[21] = 'U'; asciiValues[22] = 'V'; asciiValues[23] = 'W'; asciiValues[24] = 'X'; asciiValues[25] = 'Y';
    asciiValues[26] = 'Z'; asciiValues[36] = '_'; asciiValues[48] = '0'; asciiValues[49] = '1'; asciiValues[50] = '2';
    asciiValues[51] = '3'; asciiValues[52] = '4'; asciiValues[53] = '5'; asciiValues[54] = '6'; asciiValues[55] = '7';
    asciiValues[56] = '8'; asciiValues[57] = '9';
}

const std::string& Decoder::hexToBinary(const std::string& hexValues){
    binValues.clear();

    std::string binary;
    for (char c : hexValues) {
        // Convert each hex digit to a 6-bit binary representation
        binary += std::bitset<4>(std::stoi(std::string(1, c), nullptr, 16)).to_string();
    }
    binValues = binary;
    return binValues;
}

const std::vector<std::string>& Decoder::group6Binary(const std::string& binValues){
    binValuesGroup6.clear();

    size_t size = binValues.length();
    for(size_t i = 0; i < size; i += 6){
        std::string str = binValues.substr(i,6); // 6 bits at a time
        binValuesGroup6.push_back(str);
    }
    return binValuesGroup6;
}

const std::vector<std::string>& Decoder::binaryToDecimal(const std::vector<std::string>& binValuesGroup6){
    decValues.clear();

    size_t len = binValues.size();
     for(auto bin : binValuesGroup6){

        std::string str = std::to_string(std::stoul(bin, nullptr, 2));
        decValues.push_back(str);
    }
    return decValues;
}

void Decoder::hexToDecimal(const std::string& hexValues){
    // const auto& to assign return value
    hexToBinary(hexValues);            // convert hex string to pure binary
    group6Binary(binValues);           // convert binary into groups of 6 instead of 4
    binaryToDecimal(binValuesGroup6);  // evaluate the binary in groups of 6.
                                       // now class vector decValues will have all dec values.
}

    const std::string& Decoder::decimalToAlpha(const std::vector<std::string>& decValues){
        callSign.clear();

        for (const auto& dec : decValues) {
        int key = std::stoi(dec);

            if (asciiValues.find(key) != asciiValues.end()) {  // Check if key exists in the map
                callSign += asciiValues[key];
            } else {
                callSign += '?';  // Missing key
            }
        }
        return callSign;
    }

void Decoder::printDecValues() const{
    for(auto str : decValues)
        std::cout << str << " ";
    std::cout << std::endl;
}