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

/* AllpassReverbSeries::AllpassReverbSeries(const int& initial_delay, const double& gain, const int& number_of_blocks)
{
    blocks = new std::vector<AllpassReverbBlock>;
    previous_output = 0;
    int current_delay{initial_delay};
    int divider{1}, addition{3}; 
    for(size_t i=0; i < number_of_blocks; i++)
    {
        current_delay = initial_delay/divider;
        blocks->push_back(AllpassReverbBlock(current_delay, gain));
        divider *= 3;
    }
} 
*/

