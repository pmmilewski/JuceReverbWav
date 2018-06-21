#pragma once
#include "IReverbBlock.h"
#include "AllpassReverbBlock.h"
#include <vector>

class AllpassReverbSeries : public IReverbBlock
{
private:
    std::vector<AllpassReverbBlock> blocks;
    
public:
    AllpassReverbSeries();
    ~AllpassReverbSeries();
    
    void addBlock(const int&, const double&);
    double previous_output;
    
    int getCount();
    void clear();
    double process(const double&);
};

