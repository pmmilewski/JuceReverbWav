#include "ProcessingPipeline.h"

ProcessingPipeline::ProcessingPipeline()
{
    processingBlocks = std::vector<IReverbBlock*>();
}

ProcessingPipeline::~ProcessingPipeline()
{
    for(auto& block: processingBlocks)
    {
        delete block;
    }
}

void ProcessingPipeline::addBlockToPipeline(IReverbBlock* block)
{
    
     processingBlocks.push_back(block);
}

double ProcessingPipeline::process(const double& input_sample)
{
    double output{input_sample};
    for(auto& block: processingBlocks)
    {
        output = block->process(output);
    }
    return output;
}

void ProcessingPipeline::clear()
{
    for(auto& block: processingBlocks)
    {
        block->clear();
    }
}