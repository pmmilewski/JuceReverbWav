#include "AllpassReverbBlock.h"
#include <cmath>
#include <iostream>

AllpassReverbBlock::AllpassReverbBlock()
{
    std::cout << "Don't use AllpassReverbBlock constructor! Set parameters" << std::endl;
}

AllpassReverbBlock::~AllpassReverbBlock()
{
    
}
double AllpassReverbBlock::process(const double& input_sample)
{
    double output;
    output = input_sample*(-loop_gain) + comb->process(input_sample)*(1-pow(loop_gain, 2.0));
    return output;
}
AllpassReverbBlock::AllpassReverbBlock(const int& number_of_samples, const double& gain)
{
    loop_gain = gain;
    comb = new CombReverbBlock(number_of_samples, gain);
}

void AllpassReverbBlock::clear()
{
    comb->clear();
}
