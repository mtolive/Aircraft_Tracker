#ifndef CPR_H
#define CPR_H

#include "../include/ADSB_Message.h"
#include "../include/ExtractBits.h" 
#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>
#include <cmath>

class CPR{
// auto = std::chrono::time_point<std::chrono::system_clock>
public:
    // Number of geographic latitude zones between equator and a pole. 
    // It is set to NZ = 15 for Mode-S CPR encoding    

    CPR(ExtractBits extractor, ADSB_Message adsb) : extractor(extractor), message(adsb) {}

    std::tuple<long, long> decodePosition(std::string odd, std::string even, auto oddTimeStamp, auto evenTimeStamp);
    std::tuple<long, long> decodePosition(std::vector<uint8_t> odd, std::vector<uint8_t> even, auto oddTimeStamp, auto evenTimeStamp);
    // get methods
    double getLatEven();
    double getLatOdd();
    double getLonEven();
    double getLonOdd();
    double getNumerator();
    double getDenominator();
    double getLat();
    // set methods
    void setLatEven(const std::vector<uint8_t>& message);
    void setLatOdd(const std::vector<uint8_t>& message);
    void setLonEven(const std::vector<uint8_t>& message);
    void setLonOdd(const std::vector<uint8_t>& message);

private:
    // data
    ExtractBits& extractor;
    ADSB_Message& message;
    double numerator;
    double denominator;
    double latEven;
    double latOdd;
    int maxValue = 131072;
    // Denotes the “number of longitude zones” function, given the latitude angle lat. The returned integer value is constrained within [1, 59]
    uint8_t NL(double coordinate);
    double mod(double x, double y);
    ////////////////////////////////////////
    // even-odd latitude data + functions
    double LAT_CPR_EVEN;
    double LAT_CPR_ODD;
    double DLatE;
    double DlatO; 
    
    double computeLatE(double j, double LAT_CPR_EVEN);
    double computeLatO(double j, double LAT_CPR_ODD);
    double computeJ(double LAT_CPR_EVEN, double LAT_CPR_ODD);
    double latitude(double latitude);
    ///////////////////////////////////////
    // even-odd longitude data + functions
    double LON_CPR_EVEN;
    double LON_CPR_ODD;
    double DLon;
    double lon;

    double longitude(double DLon, double m, double ni, double LON_CPR);
    double computeNI(double lat);
    double computeDLon(double ni);
    double computeM(double LON_CPR_EVEN, double LON_CPR_ODD, double lat);
     /* Time functions*/
    double timeDifferenceInSeconds(const auto oTime, const auto eTime);
    bool timeIsGreater(const auto time1, const auto time2); 
};

#endif