#include "CombReverbBlock.h"
#include <iostream>

CombReverbBlock::CombReverbBlock()
{
    std::cout << "Don't use CombReverbBlock constructor! Set parameters" << std::endl;
}

CombReverbBlock::CombReverbBlock(const int& number_of_samples, const double& gain)
{
    loop_gain = gain;
    delay = new DelayBlock(number_of_samples);
}

CombReverbBlock::~CombReverbBlock()
{
    
}

double CombReverbBlock::process(const double &input_sample)
{
    double output;
    output = delay->process(input_sample+(delay->previous_output*loop_gain));
    return output;
}

void CombReverbBlock::clear()
{
    delay->clear();
}

