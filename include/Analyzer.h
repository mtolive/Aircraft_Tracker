#ifndef ANALYZER_H
#define ANALYZER_H

#include "ADSBFrame.h"
#include <iostream> 

class Analyzer {
private:
    ADSBFrame& frame;  // Reference to ADSBFrame

public:
    Analyzer(ADSBFrame& frameInstance) : frame(frameInstance) {}

    void analyze(const std::vector<uint8_t>& buffer); // Declaration only
    std::vector<uint8_t> hexStringToVector(std::string hexString);
    std::string trim(std::string hexString);
    void print_hex(const std::vector<uint8_t>& buffer) const;

    std::vector<uint8_t> strToByteVec(std::string hexStr);
};

#endif
