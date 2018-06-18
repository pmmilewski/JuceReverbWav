#include "DelayBlock.h"
#include <memory>
#include <iostream>

DelayBlock::DelayBlock(const int &number_of_samples)
{	
	delay_samples = number_of_samples;
	current_index = 0;
	end = delay_samples-1;
	full = false;
	buffer = std::make_unique<std::vector<double>>(number_of_samples, 0.0);
}

DelayBlock::~DelayBlock()
{
}

double DelayBlock::process(const double &input_sample) 
{
	

	double output{0};
	output = (full)? (*buffer)[current_index] : 0.0;
	(*buffer)[current_index] = input_sample;
	if(!full)
	{
		full = (current_index == end)? true : false;
	}

	current_index = (current_index == end)? 0 : current_index+1;

	previous_output = output;
	return output;
}

void DelayBlock::clear() 
{
	current_index = 0;
	end = delay_samples-1;
	full = false;
	for(auto& sample: *buffer)
	{
		sample = 0.0;
	}
}

DelayBlock::DelayBlock()
{
    std::cout << "Please don't use DelayBlock default constructor!" << std::endl;
    buffer = std::make_unique<std::vector<double>>();
}
