#ifndef FILTER_H
#define FILTER_H

#include <cstdint>
#include <string>
#include <iostream>
#include <unordered_map>
#include <map>
#include <set>
#include <vector>
#include <tuple>

class Filter{

private:

public:
    bool filterByDownlinkFormat(uint8_t df, uint8_t expectedDf);
    bool filterByTypeCode(uint8_t tc, uint8_t expectedTc);
    bool filterByTypeCode(uint8_t tc, uint8_t minTc, uint8_t maxTc);
    bool filterByIcao(const std::string& icao, const std::string& expectedIcao);
    bool filterByCallSign(const std::string& callsign, const std::string& expectedCs);
    bool filterOddEven(uint8_t oe, uint8_t expectedOe);
    bool filterIdMsg(uint8_t df, uint8_t tc);
    bool filterOddMsg(uint8_t df, uint8_t tc, uint8_t oe);
    bool filterEvenMsg(uint8_t df, uint8_t tc, uint8_t oe);
    bool filterPosMsg(uint8_t df, uint8_t tc);
    bool filterVelocityMsg(uint8_t df, uint8_t tc);



};

#endif