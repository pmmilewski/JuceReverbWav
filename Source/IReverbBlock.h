#pragma once 
class IReverbBlock
{

public:

    virtual double process(const double&) = 0;
	virtual void clear() = 0;
};
