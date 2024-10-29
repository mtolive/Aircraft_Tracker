#include "../include/ADSBMessage.h"

const size_t ADSBMessage::FRAME_SIZE = 14;

void ADSBMessage::initializeFromBuffer(const std::vector<uint8_t>& buffer) {
    if (buffer.size() >= getFrameSize()) {
        /*
         * These three values are common to all ADSB messages
         * the other values are conditional and will be dependent
         * on these valuese DF, TC, ICAO
        */
        setBuffer(buffer);
        extractDf(buffer);
        extractTc(buffer);
        extractIcao(buffer);
    } else {
        throw std::invalid_argument("Invalid frame size");
    }
}


/* Extraction Functions*/

void ADSBMessage::extractIcao(const std::vector<uint8_t>& message){
    setIcao(decoder.decodeIcao(message));
}

void ADSBMessage::extractTc(const std::vector<uint8_t>& message){
    setTypeCode(decoder.decodeTypeCode(message));
}
void ADSBMessage::extractDf(const std::vector<uint8_t>& message){
    setDownlinkFormat(decoder.decodeDownlinkFormat(message));
}

void ADSBMessage::extractOe(const std::vector<uint8_t>& message){
    setOddEven(decoder.decodeOddEven(message));
}

void ADSBMessage::extractCallsign(const std::vector<uint8_t>& message){
    setCallsign(decoder.decodeCallSign(message));
}

// will add velocity one day

/**** Set Functions ****/
void ADSBMessage::setBuffer(const std::vector<uint8_t>& buffer) {
    message = buffer; // rawData is still a class member variable
}

void ADSBMessage::setDownlinkFormat(const uint8_t df) {
    this->df = df; // Access the df member inside the ADSBData struct
}

void ADSBMessage::setTypeCode(const uint8_t tc) {
    this->tc = tc; // Access the tc member inside the ADSBData struct
}

void ADSBMessage::setIcao(const std::string& icao) {
    this->icao = std::move(icao); // Access the icao member inside the ADSBData struct
}

void ADSBMessage::setCallsign(const std::string& callsign) {
    this->callsign = std::move(callsign); // Access the callsign member inside the ADSBData struct
}

void ADSBMessage::setOddEven(const uint8_t oddEven){
    this->oddEven = oddEven;
}

/**** Get Functions ****/
const std::vector<uint8_t>& ADSBMessage::getMessage() const{
    return message;
}

const uint8_t ADSBMessage::getDownlinkFormat() const {
    return df;
}

const uint8_t ADSBMessage::getTypeCode() const {
    return tc;
}

const std::string& ADSBMessage::getIcao() const {
    return icao; 
}

const std::string& ADSBMessage::getCallsign() const {
    return callsign;
}

const uint8_t ADSBMessage::getOddEven() const{
    return oddEven;    
}

const size_t ADSBMessage::getFrameSize() const {
    return FRAME_SIZE; // Assuming this is a class constant and hasn't changed
}
