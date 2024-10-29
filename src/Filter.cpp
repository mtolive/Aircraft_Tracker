#include "../include/Filter.h"

bool Filter::filterByDownlinkFormat(uint8_t df, uint8_t expectedDf) {
    return df == expectedDf;
}

bool Filter::filterByTypeCode(uint8_t tc, uint8_t expectedTc) {
    return tc == expectedTc;
}

bool Filter::filterByTypeCode(uint8_t tc, uint8_t minTc, uint8_t maxTc) {
    return tc >= minTc && tc <= maxTc;
}

bool Filter::filterByIcao(const std::string& icao, const std::string& expectedIcao) {
    return icao.find(expectedIcao) != std::string::npos;
}

bool Filter::filterByCallSign(const std::string& callsign, const std::string& expectedCs) {
    return callsign.find(expectedCs) != std::string::npos;
}

bool Filter::filterOddEven(uint8_t oe, uint8_t expectedOe){
    return oe == expectedOe;
}

bool Filter::filterIdMsg(uint8_t df, uint8_t tc){
    return filterByDownlinkFormat(df, 17) && filterByTypeCode(tc, 1, 4);
}

bool Filter::filterOddMsg(uint8_t df, uint8_t tc, uint8_t oe){
    return (filterByDownlinkFormat(df, 17) && filterByTypeCode(tc, 9, 18)
            && filterOddEven(oe, 1));
}

bool Filter::filterEvenMsg(uint8_t df, uint8_t tc, uint8_t oe){
    return (filterByDownlinkFormat(df, 17) && filterByTypeCode(tc, 9, 18)
            && filterOddEven(oe, 0));
}

bool Filter::filterPosMsg(uint8_t df, uint8_t tc){
    return (filterByDownlinkFormat(df, 17) && filterByTypeCode(tc, 9, 18));
}