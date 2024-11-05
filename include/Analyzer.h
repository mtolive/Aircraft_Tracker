#ifndef ANALYZER_H
#define ANALYZER_H

#include "ADSBMessage.h"
#include "Filter.h"
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>
#include <cstdint>
#include <iomanip>
#include <map>
#include <tuple>
#include <unordered_map> // for std::unordered_map
#include <unordered_set>
#include <set> // for std::set

class Analyzer {
private: 
    typedef struct{
        std::string icao;
        std::vector<uint8_t> message;
        std::string callsign;
        uint8_t oe;
        std::chrono::time_point<std::chrono::system_clock> timestamp;
    } Data;
    
    ADSBMessage& message;  // Reference to ADSBFrame
    Filter& filter;        // Refernce to Filter
    std::unordered_map<std::string, Data> storedIcaos;
    std::unordered_map<std::string, std::tuple<Data, Data>> posMessages;
    //Data data;

    /* Initialize Data */
    Data initData(ADSBMessage& message);
    void initEvenMsg(ADSBMessage& message, std::tuple<Data, Data>& tuple);
    void initOddMsg(ADSBMessage& message, std::tuple<Data, Data>& tuple);
    //void initTupleOe(std::tuple<Data, Data> tuple);

    /* Analyze Message Function */
    const std::vector<uint8_t>& analyzeIdMsg(ADSBMessage& message);
    const std::vector<uint8_t>& analyzePosMsg(ADSBMessage& message);
    const std::vector<uint8_t>& analyzeVelMsg(ADSBMessage& message);

    /* Time functions*/
    double timeDifferenceInSeconds(const Data& data1, const Data& data2);

public:
    // Inject ADSBMessage and Filter dependencies
    Analyzer(ADSBMessage& messageInstance, Filter& filterInstance) : message(messageInstance), filter(filterInstance) {}

    /* Main Logic Function*/
    void analyze(const std::vector<uint8_t>& buffer); // Declaration only

    /* Format Functions */
    std::vector<uint8_t> hexStringToVector(std::string hexString);
    std::string trim(std::string hexString);
    std::vector<uint8_t> strToByteVec(std::string hexStr);
    void print_hex(const std::vector<uint8_t>& buffer) const;
};
#endif
