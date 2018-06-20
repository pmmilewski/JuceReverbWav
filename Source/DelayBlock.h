#pragma once
#include <vector>

class DelayBlock {
private:
	int delay_samples;
	int current_index;
	bool full;
	int end;
	std::vector<double> buffer;
    
public:
    DelayBlock();
	DelayBlock(const int&);
	~DelayBlock();

	double process(const double&);
	void clear();

    
    double previous_output; // will be used for loops
	
};
