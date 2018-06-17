#include "AllpassReverbSeries.h"
#include <iostream>

AllpassReverbSeries::AllpassReverbSeries()
{
    blocks = nullptr;
    previous_output = 0;
}

AllpassReverbSeries::~AllpassReverbSeries()
{
    delete blocks;
}

void AllpassReverbSeries::addBlock(const int& delay, const double& gain)
{
    if(blocks == nullptr)
    {
        blocks = new std::vector<AllpassReverbBlock>({AllpassReverbBlock(delay, gain)});
    }
    else
    {
        blocks->push_back({AllpassReverbBlock(delay, gain)});
    }
} 

double AllpassReverbSeries::process(const double& input_sample)
{
    double output{input_sample};
    for(auto& block: *blocks)
    {
        output = block.process(output);
    }
    previous_output = output;
    return output;
}

int AllpassReverbSeries::getCount()
{
    if(blocks != nullptr)
        return blocks->size();
    else
        return 0;
}



