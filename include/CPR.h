#ifndef CPR_H
#define CPR_H

#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>
#include <cmath>

class CPR{
// auto = std::chrono::time_point<std::chrono::system_clock>
public:
    // Number of geographic latitude zones between equator and a pole. It is set to NZ = 15 for Mode-S CPR encoding
    const uint8_t NZ;

    std::tuple<long, long> decodePosition(std::string odd, std::string even, auto oddTimeStamp, auto evenTimeStamp);
    std::tuple<long, long> decodePosition(std::vector<uint8_t> odd, std::vector<uint8_t> even, auto oddTimeStamp, auto evenTimeStamp);
private:
    // Denotes the “number of longitude zones” function, given the latitude angle lat. The returned integer value is constrained within [1, 59]
    uint8_t NL(double coordinate);
    double longitude(double longitude);
    double latitude(double latitude);
     /* Time functions*/
    double timeDifferenceInSeconds(const auto oTime, const auto eTime);
    bool timeIsGreater(const auto time1, const auto time2); 
};

#endif