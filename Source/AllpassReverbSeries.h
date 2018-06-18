#pragma once
#include "AllpassReverbBlock.h"
#include <vector>

class AllpassReverbSeries
{
private:
    std::vector<AllpassReverbBlock>* blocks;
    
public:
    AllpassReverbSeries();
    //AllpassReverbSeries(const int&, const double&, const int&);
    ~AllpassReverbSeries();
    
    void addBlock(const int&, const double&);
    double previous_output;
    
    int getCount();
    void clear();
    double process(const double&);
};

