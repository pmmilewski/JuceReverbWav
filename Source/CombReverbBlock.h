#pragma once
#include "IReverbBlock.h"
#include "DelayBlock.h"

class CombReverbBlock : public IReverbBlock
{
private:
    DelayBlock delay;
    double loop_gain;
    
public:
    CombReverbBlock();
    CombReverbBlock(const int&, const double&);
    ~CombReverbBlock();

    void clear();
    double process(const double&);
};

