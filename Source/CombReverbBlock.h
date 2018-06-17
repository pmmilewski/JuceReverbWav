#pragma once
#include "DelayBlock.h"

class CombReverbBlock
{
private:
    DelayBlock* delay;
    double loop_gain;
    
public:
    CombReverbBlock();
    CombReverbBlock(const int&, const double&);
    ~CombReverbBlock();

    void clear();
    double process(const double&);
};

