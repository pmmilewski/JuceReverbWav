#include "IReverbBlock.h"
#include <memory>
#include <vector>

//using uptr = std::unique_ptr<IReverbBlock>;

class ProcessingPipeline
{
private:
    std::vector<IReverbBlock*> processingBlocks;
public:
    ProcessingPipeline();
    ~ProcessingPipeline();

    void addBlockToPipeline(IReverbBlock*);
    void clear();
    double process(const double&);
};
