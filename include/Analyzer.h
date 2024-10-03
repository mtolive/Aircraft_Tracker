#ifndef ANALYZER_H
#define ANALYZER_H

#include "../include/ADSBFrame.h"
#include "../include/Filter.h"
#include <iostream>


class Analyzer {
public:
    void analyze(const std::vector<uint8_t>& buffer);
};

#endif // ANALYZER_H
