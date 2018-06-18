#include "CombReverbParallel.h"

CombReverbParallel::CombReverbParallel()
{
    blocks = nullptr;
}

CombReverbParallel::~CombReverbParallel()
{
    delete blocks;
}

void CombReverbParallel::addBlock(const int& delay, const double& gain)
{
    if(blocks == nullptr)
    {
        blocks = new std::vector<CombReverbBlock>({CombReverbBlock(delay, gain)});
    }
    else
    {
        blocks->push_back({CombReverbBlock(delay, gain)});
    }
} 

double CombReverbParallel::process(const double& input_sample)
{
    double output{0};
    for(auto& block: *blocks)
    {
        output += block.process(input_sample);
    }
    output /= blocks->size();
    return output;
}

int CombReverbParallel::getCount()
{
    if(blocks != nullptr)
        return blocks->size();
    else
        return 0;
}

void CombReverbParallel::clear()
{
    for(auto& block: *blocks)
    {
        block.clear();
    }
}