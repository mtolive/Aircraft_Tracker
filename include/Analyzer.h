#ifndef ANALYZER_H
#define ANALYZER_H

#include "ADSBMessage.h"
#include "Filter.h"
#include <iostream> 

class Analyzer {
private:
    ADSBMessage& message;  // Reference to ADSBFrame
    Filter& filter;        // Refernce to Filter

    struct Data{
        std::string icao;
        std::vector<uint8_t> message;
        std::string callsign;
        uint8_t oe;
    };

    std::unordered_map<std::string, Data> storedIcaos;
    Data data;
public:
    // Inject ADSBMessage and Filter dependencies
    Analyzer(ADSBMessage& messageInstance, Filter& filterInstance) : message(messageInstance), filter(filterInstance) {}

    /* Main Logic Function*/
    void analyze(const std::vector<uint8_t>& buffer); // Declaration only

    /* Set Methods */
    void setIcao(const std::string& icao);
    void setMessage(const std::vector<uint8_t>& message);
    void setCallsign(const std::string& callsign);
    void setOe(const uint8_t oe);

    /* Get Functions */
    const std::string& getIcao() const;
    const std::vector<uint8_t>& getMessage() const;
    const std::string& getCallsign() const;
    const uint8_t getOe() const;

    /* Format Functions */
    std::vector<uint8_t> hexStringToVector(std::string hexString);
    std::string trim(std::string hexString);
    std::vector<uint8_t> strToByteVec(std::string hexStr);
    void print_hex(const std::vector<uint8_t>& buffer) const;


};

#endif
