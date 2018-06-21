#pragma once
#include "IReverbBlock.h"
#include "CombReverbBlock.h"

class AllpassReverbBlock : public IReverbBlock
{
private:
    CombReverbBlock comb;
    double loop_gain;
    
public:
    AllpassReverbBlock();
    AllpassReverbBlock(const int&, const double&);
    ~AllpassReverbBlock();
    
    void clear() override;
    double process(const double&) override;
};
