#include "../include/Filter.h"

/**** All messages will have Downlink Format == 17 ****/

// Filter for downlink format
bool Filter::filterByDownlinkFormat(uint8_t df, uint8_t expectedDf) {
    return df == expectedDf;
}
// Filter for type code
bool Filter::filterByTypeCode(uint8_t tc, uint8_t expectedTc) {
    return tc == expectedTc;
}
// Filter for type code (range) - inclusive
bool Filter::filterByTypeCode(uint8_t tc, uint8_t minTc, uint8_t maxTc) {
    return tc >= minTc && tc <= maxTc;
}
// Filter for ICAO - args: hex string references
bool Filter::filterByIcao(const std::string& icao, const std::string& expectedIcao) {
    return icao.find(expectedIcao) != std::string::npos;
}
// Filter for call sign - args: hex string references
bool Filter::filterByCallSign(const std::string& callsign, const std::string& expectedCs) {
    return callsign.find(expectedCs) != std::string::npos;
}
// Filter for odd/even position messages
bool Filter::filterOddEven(uint8_t oe, uint8_t expectedOe){
    return oe == expectedOe;
}
// Filter for identification message: type codes 1-4 inclusive
bool Filter::filterIdMsg(uint8_t df, uint8_t tc){
    return filterByDownlinkFormat(df, 17) && filterByTypeCode(tc, 1, 4);
}
// Filter for ODD position message: odd/even == 1, type codes 9-18 inclusive
bool Filter::filterOddMsg(uint8_t df, uint8_t tc, uint8_t oe){
    return (filterByDownlinkFormat(df, 17) && filterByTypeCode(tc, 9, 18)
            && filterOddEven(oe, 1));
}
// Filter for EVEN position message: odd/even == 0, type codes 9-18 inclusive
bool Filter::filterEvenMsg(uint8_t df, uint8_t tc, uint8_t oe){
    return (filterByDownlinkFormat(df, 17) && filterByTypeCode(tc, 9, 18)
            && filterOddEven(oe, 0));
}
// Filter for position message: type codes 9-18 inclusive
bool Filter::filterPosMsg(uint8_t df, uint8_t tc){
    return (filterByDownlinkFormat(df, 17) && filterByTypeCode(tc, 9, 18));
}
// Filter for velocity message: type code == 19
bool Filter::filterVelocityMsg(uint8_t df, uint8_t tc){
    return (filterByDownlinkFormat(df, 17) && filterByTypeCode(tc, 19));
}
