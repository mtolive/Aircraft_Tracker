#ifndef ADSB_CONTROLLER_H
#define ADSB_CONTROLLER_H

#include "/home/m/Aircraft_Tracker/include/interfaces/Controller.h"
#include "ADSB_Message.h"
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

class ADSB_Controller : public Controller {
private: 
    typedef struct{
        std::string icao;
        std::vector<uint8_t> message;
        std::string callsign;
        uint8_t oe;
        std::chrono::time_point<std::chrono::system_clock> timestamp;
    } Data;
    
    
    ADSB_Message& message;  // Reference to ADSBFrame
    Filter& filter;        // Refernce to Filter
    std::unordered_map<std::string, Data> storedIcaos;
    std::unordered_map<std::string, std::tuple<Data, Data>> posMessages;
    //Data data;

    /* Initialize Data */
    Data initData(ADSB_Message& message);
    void initEvenMsg(ADSB_Message& message, std::tuple<Data, Data>& tuple);
    void initOddMsg(ADSB_Message& message, std::tuple<Data, Data>& tuple);
    //void initTupleOe(std::tuple<Data, Data> tuple);

    /* Analyze Message Function */
    const std::vector<uint8_t>& analyzeIdMsg(ADSB_Message& message);
    const std::vector<uint8_t>& analyzePosMsg(ADSB_Message& message);
    const std::vector<uint8_t>& analyzeVelMsg(ADSB_Message& message);

    /* Time functions*/
    double timeDifferenceInSeconds(const Data& data1, const Data& data2);

public:
    // Inject ADSBMessage and Filter dependencies
    ADSB_Controller(ADSB_Message& messageInstance, Filter& filterInstance) : message(messageInstance), filter(filterInstance) {}

    /* Main Logic Function*/
    void analyze(const std::vector<uint8_t>& buffer) override; // Declaration only

    /* Format Functions */
    std::vector<uint8_t> hexStringToVector(std::string hexString);
    std::string trim(std::string hexString);
    std::vector<uint8_t> strToByteVec(std::string hexStr);
    void print_hex(const std::vector<uint8_t>& buffer) const;
};
#endif
