#pragma once
#include "IReverbBlock.h"
#include "CombReverbBlock.h"
#include <vector>

class CombReverbParallel : public IReverbBlock
{
private:
    std::vector<CombReverbBlock> blocks;
    
public:
    CombReverbParallel();
    ~CombReverbParallel();
    
    void addBlock(const int&, const double&);
    
    int getCount();
    void clear();
    double process(const double&);
};