#include "AllpassReverbSeries.h"
#include <iostream>

AllpassReverbSeries::AllpassReverbSeries()
{
    blocks = std::vector<AllpassReverbBlock>();
    previous_output = 0;
}

AllpassReverbSeries::~AllpassReverbSeries()
{
}

void AllpassReverbSeries::addBlock(const int& delay, const double& gain)
{
        blocks.push_back({AllpassReverbBlock(delay, gain)});
} 

double AllpassReverbSeries::process(const double& input_sample)
{
    double output{input_sample};
    for(auto& block: blocks)
    {
        output = block.process(output);
    }
    previous_output = output;
    return output;
}

int AllpassReverbSeries::getCount()
{
    return blocks.size();
}

void AllpassReverbSeries::clear()
{
    for(auto& block: blocks)
    {
        block.clear();
    }
}