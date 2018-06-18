#pragma once
#include <vector>
#include <memory>

class DelayBlock {
private:
	int delay_samples;
	int current_index;
	bool full;
	int end;
	std::unique_ptr<std::vector<double>> buffer = nullptr;
    
public:
    DelayBlock();
	DelayBlock(const int&);
	~DelayBlock();

	double process(const double&);
	void clear();

    
    double previous_output; // will be used for loops
	
};
