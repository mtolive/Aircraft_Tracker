#include "../include/ADSB_Message.h"

const size_t ADSB_Message::FRAME_SIZE = 14;

void ADSB_Message::initializeFromBuffer(const std::vector<uint8_t>& buffer) {
    if (buffer.size() >= getFrameSize()) {
        /*
         * These three values are common to all ADSB messages
         * the other values are conditional and will be dependent
         * on these valuese DF, TC, ICAO
        */
        setBuffer(buffer);    // 14-byte hex message
        extractDf(buffer);    // DF
        extractTc(buffer);    // TC
        extractIcao(buffer);  // ICAO
    } else {
        throw std::invalid_argument("Invalid frame size");
    }
}

/* Extraction Functions (decoder wrapper functions) - Set class variables
**  ICAO (unique aircraft identifier)
**  TC (type code) - inidicates what type of ADSB message is recieved
**  DF (downlink file) - inidicates if ADSB, non adsb (short long)
**  OE (Odd even) indicates if ADSB ID message is odd or even
**  CS (Call sign) extracted from ADSB ID messages (DF == 17 && TC is in range 1-4 inclusive)
*/
void ADSB_Message::extractIcao(const std::vector<uint8_t>& message){
    setIcao(decoder.decodeIcao(message));
}

void ADSB_Message::extractTc(const std::vector<uint8_t>& message){
    setTypeCode(decoder.decodeTypeCode(message));
}
void ADSB_Message::extractDf(const std::vector<uint8_t>& message){
    setDownlinkFormat(decoder.decodeDownlinkFormat(message));
}

void ADSB_Message::extractOe(const std::vector<uint8_t>& message){
    setOddEven(decoder.decodeOddEven(message));
}

void ADSB_Message::extractCallsign(const std::vector<uint8_t>& message){
    setCallsign(decoder.decodeCallSign(message));
}

/**** Set Functions ****/
void ADSB_Message::setBuffer(const std::vector<uint8_t>& message) {
    this->message = message; 
} 

void ADSB_Message::setDownlinkFormat(const uint8_t df) {
    this->df = df; 
}

void ADSB_Message::setTypeCode(const uint8_t tc) {
    this->tc = tc; 
}

void ADSB_Message::setIcao(const std::string& icao) {
    this->icao = std::move(icao); 
}

void ADSB_Message::setCallsign(const std::string& callsign) {
    this->callsign = std::move(callsign); 
}

void ADSB_Message::setOddEven(const uint8_t oddEven){
    this->oddEven = oddEven;
}

/**** Get Functions ****/
const std::vector<uint8_t>& ADSB_Message::getMessage() const{
    return message;
}

const uint8_t ADSB_Message::getDownlinkFormat() const {
    return df;
}

const uint8_t ADSB_Message::getTypeCode() const {
    return tc;
}

const std::string& ADSB_Message::getIcao() const {
    return icao; 
}

const std::string& ADSB_Message::getCallsign() const {
    return callsign;
}

const uint8_t ADSB_Message::getOddEven() const{
    return oddEven;    
}

const size_t ADSB_Message::getFrameSize() const {
    return FRAME_SIZE; // Will always be 14, until non ADSB messages are added
}
