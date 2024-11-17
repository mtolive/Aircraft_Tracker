#ifndef CONTROLLER_I
#define CONTROLLER_I

#include <iostream>
#include <vector>

class Controller{
public:
    virtual void analyze(const std::vector<uint8_t>& buffer) = 0; // Declaration only
};

#endif