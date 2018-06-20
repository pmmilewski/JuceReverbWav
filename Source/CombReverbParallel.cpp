#include "CombReverbParallel.h"

CombReverbParallel::CombReverbParallel()
{
    blocks = std::vector<CombReverbBlock>();
}

CombReverbParallel::~CombReverbParallel()
{
}

void CombReverbParallel::addBlock(const int& delay, const double& gain)
{
        blocks.push_back({CombReverbBlock(delay, gain)});
} 

double CombReverbParallel::process(const double& input_sample)
{
    double output{0};
    for(auto& block: blocks)
    {
        output += block.process(input_sample);
    }
    output /= blocks.size();
    return output;
}

int CombReverbParallel::getCount()
{
    return blocks.size();
}

void CombReverbParallel::clear()
{
    for(auto& block: blocks)
    {
        block.clear();
    }
}