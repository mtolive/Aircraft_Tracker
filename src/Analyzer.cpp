#include "../include/Analyzer.h"

void Analyzer::analyze(const std::vector<uint8_t>& buffer) {
    try {
        ADSBFrame frame(buffer);  // Create an ADSBFrame instance
        
        // Apply filters using the Filter class
        if (Filter::filterByDownlinkFormat(frame, 17)) {
            //frame.printHex();  // Print only if it matches the DF filter
            if (Filter::filterByTypeCode(frame, 1,4)){
                std::cout << frame.getCallSign() << std::endl;
            }

        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error analyzing frame: " << e.what() << std::endl;
    }
}
