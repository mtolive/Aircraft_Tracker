#include "../include/CPR.h"

/*
    ADSB_Message set time stamp and in interface as well
*/
/*
    for both even and odd timestamps: auto = std::chrono::time_point<std::chrono::system_clock>

*/

// private
const uint8_t NZ = 15;
double lat = 0.0;
double numerator = (2*M_PI);
double denominator = acos(
    1 - (1-cos(M_PI / (2*NZ))) /
        (pow(cos((M_PI/180)*lat), 2))
                           
);

std::tuple<long, long> CPR::decodePosition(std::string odd, std::string even, auto oddTimeStamp, auto evenTimeStamp){


}

std::tuple<long, long> CPR::decodePosition(std::vector<uint8_t> odd, std::vector<uint8_t> even, auto oddTimeStamp, auto evenTimeStamp){


}


uint8_t CPR::NL(double coordinate){

//floor()

}

// if time1 is greater than time2
bool CPR::timeIsGreater(const auto time1, const auto time2){
    
    return time1 > time2;

}

double CPR::timeDifferenceInSeconds(const auto timestamp1, const auto timestamp2){
    auto duration =  timestamp1 - timestamp2;
    return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
}
