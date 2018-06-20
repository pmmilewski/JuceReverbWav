#pragma once
#include "CombReverbBlock.h"

class AllpassReverbBlock
{
private:
    CombReverbBlock comb;
    double loop_gain;
    
public:
    AllpassReverbBlock();
    AllpassReverbBlock(const int&, const double&);
    ~AllpassReverbBlock();
    
    void clear();
    double process(const double&);
};
