#include "WaveFileWrapper.h"
#include <bitset>
#include <cmath>
#include <algorithm>


void WaveFileWrapper::readFile(const char* filename)
{
	std::ifstream fs;
	wave = new WaveFile;
	fs.open(filename, std::ios_base::binary);
    fs.read(reinterpret_cast<char*>(&wave->header), 36);

    wave->header.ExtraData = new std::vector<uint8_t>;
    uint8_t byte;
    int32_t header_byte_number{36};
    while (header_byte_number < wave->header.Subchunk1Size+20)
    {
        fs.seekg(header_byte_number);
        fs.read(reinterpret_cast<char *>(&byte), 1);
        wave->header.ExtraData->push_back(byte);
        header_byte_number++;
    }
    fs.seekg(header_byte_number);
    fs.read(reinterpret_cast<char*>(&wave->header.Subchunk2ID), 8);

    bps = static_cast<int>(wave->header.BitsPerSample);
    if(bps == 8)
        readSamplesFromFile<8>(fs);
    if(bps == 16)
        readSamplesFromFile<16>(fs);
    if(bps == 24)
        readSamplesFromFile<24>(fs);
    if(bps == 32)
        readSamplesFromFile<32>(fs);
	fs.close();
}

void WaveFileWrapper::writeFile(const char* filename)
{
	std::ofstream fs;
	fs.open(filename, std::ios_base::binary);
	//fs.write((char*)wave, sizeof(wave->header));
    fs.write(reinterpret_cast<char*>(&wave->header), 36);
    int header_byte_number{36};
    for(auto& byte: *wave->header.ExtraData)
    {
        fs.write(reinterpret_cast<char*>(&byte), 1);
        header_byte_number++;
    }
    fs.write(reinterpret_cast<char*>(&wave->header.Subchunk2ID), 8);

    if(bps == 8)
        writeSamplesToFile<8>(fs);
    if(bps == 16)
        writeSamplesToFile<16>(fs);
    if(bps == 24)
        writeSamplesToFile<24>(fs);
    if(bps == 32)
        writeSamplesToFile<32>(fs);
	fs.close();
}
WaveFileWrapper::WaveFileWrapper(const char* filename)
{
    readFile(filename);
    // Need to consider additional init operations
    number_of_channels = static_cast<int>(wave->header.NumChannels);
    block_align = static_cast<int>(wave->header.BlockAlign);
    sample_rate = static_cast<int>(wave->header.SampleRate);
}

void WaveFileWrapper::finishWork(const char* filename)
{
    writeFile(filename);
}

SoundData* WaveFileWrapper::getSoundData()
{
    SoundData *sdata = new SoundData{0};
    sdata->number_of_channels = number_of_channels;
    sdata->sample_rate = sample_rate;
    sdata->bitrate = bps;
    
    switch(number_of_channels)
    {
        case 1:
        {
            if(bps == 8)
            {
                auto* data = (std::vector<uint8_t>*)wave->data;
                sdata->left_channel = std::vector<double>(data->begin(), data->end());
            }
            if(bps == 16)
            {
                auto* data = (std::vector<int16_t>*)wave->data;
                sdata->left_channel = std::vector<double>(data->begin(), data->end());
            }
            if(bps == 24)
            {
                auto* data = (std::vector<int24_t>*)wave->data;
                sdata->left_channel = std::vector<double>(data->begin(), data->end());
            }
            if(bps == 32)
            {
                auto* data = (std::vector<int32_t>*)wave->data;
                sdata->left_channel = std::vector<double>(data->begin(), data->end());
            }
            break;
        }
        case 2:
        {
            if(bps == 8)
            {
                auto* data = (std::vector<uint8_t>*)wave->data;
                size_t data_size = data->size();
                
                for(size_t i = 0; i < data_size; i++)
                {
                    (i%2 == 0)? 
                    sdata->left_channel.push_back((*data)[i]) : sdata->right_channel.push_back((*data)[i]);
                }
            }
            if(bps == 16)
            {
                auto* data = (std::vector<int16_t>*)wave->data;
                size_t data_size = data->size();

                for(size_t i = 0; i < data_size; i++)
                {
                    (i%2 == 0)? 
                    sdata->left_channel.push_back((*data)[i]) : sdata->right_channel.push_back((*data)[i]);
                }
            }
            if(bps == 24)
            {
                auto* data = (std::vector<int24_t>*)wave->data;
                size_t data_size = data->size();
                for(size_t i = 0; i < data_size; i++)
                {
                    (i%2 == 0)? 
                    sdata->left_channel.push_back((*data)[i]) : sdata->right_channel.push_back((*data)[i]);
                }
            }
            if(bps == 32)
            {
                auto* data = (std::vector<int32_t>*)wave->data;
                size_t data_size = data->size();
                
                for(size_t i = 0; i < data_size; i++)
                {
                    (i%2 == 0)? 
                    sdata->left_channel.push_back((*data)[i]) : sdata->right_channel.push_back((*data)[i]);
                }
            }
            break;
        }
        default:
        {
            std::cout << "Something is not right with number of channels in getSoundData()" << std::endl;
        }
    }
    sdata->initial_size = sdata->left_channel.size();
    return sdata;
        
}

void WaveFileWrapper::loadSoudData(SoundData& sdata)
{
    switch(number_of_channels)
    {
        case 1:
        {
            auto *left = &sdata.left_channel;
            auto *right = &sdata.right_channel;

            auto l_minmax = std::minmax_element(left->begin(), left->end());
            auto l_min = *l_minmax.first;
            auto l_max = *l_minmax.second;

            if (bps == 8)
            {
                delete static_cast<std::vector<uint8_t>*>(wave->data);
                
                std::transform(left->begin(), left->end(), left->begin(),
                [&l_max, &l_min](double s) -> double {return (s - l_min)/(l_max - l_min)*UINT8_MAX;});
                
                wave->data = new std::vector<uint8_t>(left->begin(), left->end());
            }
            if (bps == 16)
            {
                delete static_cast<std::vector<int16_t> *>(wave->data);

                std::transform(left->begin(), left->end(), left->begin(),
                [&l_max, &l_min](double s) -> double {return (((s - l_min)/(l_max - l_min)*2.0)-1.0)*INT16_MAX;});

                wave->data = new std::vector<int16_t>(left->begin(), left->end());
            }
            if (bps == 24)
            {
                delete static_cast<std::vector<int24_t> *>(wave->data);

                std::transform(left->begin(), left->end(), left->begin(),
                [&l_max, &l_min](double s) -> double {return (((s - l_min)/(l_max - l_min)*2.0)-1.0)*INT24_MAX;});

                wave->data = new std::vector<int24_t>(left->begin(), left->end());
            }
            if (bps == 32)
            {
                delete static_cast<std::vector<int32_t> *>(wave->data);

                std::transform(left->begin(), left->end(), left->begin(),
                [&l_max, &l_min](double s) -> double {return (((s - l_min)/(l_max - l_min)*2.0)-1.0)*INT32_MAX;});

                wave->data = new std::vector<int32_t>(left->begin(), left->end());
            }
            break;
        }
        case 2:
        {
            
            auto *left = &sdata.left_channel;
            auto *right = &sdata.right_channel;
            

            auto l_minmax = std::minmax_element(left->begin(), left->end());
            auto l_min = *l_minmax.first;
            auto l_max = *l_minmax.second;

            auto r_minmax = std::minmax_element(right->begin(), right->end());
            auto r_min = *r_minmax.first;
            auto r_max = *r_minmax.second;
            
            if (bps == 8)
            {
                auto* data = static_cast<std::vector<uint8_t>*>(wave->data);
                delete data;
                size_t data_size = left->size();

                std::transform(left->begin(), left->end(), left->begin(),
                [&l_max, &l_min](double s) -> double {return (s - l_min)/(l_max - l_min)*UINT8_MAX;});

                 std::transform(right->begin(), right->end(), right->begin(),
                [&r_max, &r_min](double s) -> double {return (s - r_min)/(r_max - r_min)*UINT8_MAX;});
                
                wave->data = new std::vector<uint8_t>;
                data = static_cast<std::vector<uint8_t>*>(wave->data);
                
                for(size_t i = 0; i < data_size; i++)
                {
                    data->push_back((*left)[i]);
                    data->push_back((*right)[i]);
                }
                
            }
            if (bps == 16)
            {
                auto* data = static_cast<std::vector<int16_t>*>(wave->data);
                delete data;
                size_t data_size = left->size();

                std::transform(left->begin(), left->end(), left->begin(),
                [&l_max, &l_min](double s) -> double {return (((s - l_min)/(l_max - l_min)*2.0)-1.0)*INT16_MAX;});

                 std::transform(right->begin(), right->end(), right->begin(),
                [&r_max, &r_min](double s) -> double {return (((s - r_min)/(r_max - r_min)*2.0)-1.0)*INT16_MAX;});
                
                wave->data = new std::vector<int16_t>;
                data = static_cast<std::vector<int16_t>*>(wave->data);
                
                for(size_t i = 0; i < data_size; i++)
                {
                    data->push_back((*left)[i]);
                    data->push_back((*right)[i]);
                }
            }
            if (bps == 24)
            {
                auto* data = static_cast<std::vector<int24_t>*>(wave->data);
                delete data;
                size_t data_size = left->size();

                std::transform(left->begin(), left->end(), left->begin(),
                [&l_max, &l_min](double s) -> double {return (((s - l_min)/(l_max - l_min)*2.0)-1.0)*INT24_MAX;});

                 std::transform(right->begin(), right->end(), right->begin(),
                [&r_max, &r_min](double s) -> double {return (((s - r_min)/(r_max - r_min)*2.0)-1.0)*INT24_MAX;});
                
                wave->data = new std::vector<int24_t>;
                data = static_cast<std::vector<int24_t>*>(wave->data);
                
                for(size_t i = 0; i < data_size; i++)
                {
                    data->push_back(int24_t((*left)[i]));
                    data->push_back(int24_t((*right)[i]));
                }
            }
            if (bps == 32)
            {
                auto* data = static_cast<std::vector<int32_t>*>(wave->data);
                delete data;
                size_t data_size = left->size();

                std::transform(left->begin(), left->end(), left->begin(),
                [&l_max, &l_min](double s) -> double {return (((s - l_min)/(l_max - l_min)*2.0)-1.0)*INT32_MAX;});

                std::transform(right->begin(), right->end(), right->begin(),
                [&r_max, &r_min](double s) -> double {return (((s - r_min)/(r_max - r_min)*2.0)-1.0)*INT32_MAX;});
                
                wave->data = new std::vector<int32_t>;
                data = static_cast<std::vector<int32_t>*>(wave->data);
                
                for(size_t i = 0; i < data_size; i++)
                {
                    data->push_back((*left)[i]);
                    data->push_back((*right)[i]);
                }
            }
            break;
        }
        default:
        {
            std::cout << "Something is not right with number of channels in getSoundData()" << std::endl;
        }
    }
}
WaveFileWrapper::WaveFileWrapper(const SoundData& sdata)
{
    wave = new WaveFile;
    
    sample_rate = sdata.sample_rate;
    number_of_channels = sdata.number_of_channels;
    bps = sdata.bitrate;
    block_align = number_of_channels * bps/8;
    
    wave->header.ChunkID[0] = 'R';
    wave->header.ChunkID[1] = 'I';
    wave->header.ChunkID[2] = 'F';
    wave->header.ChunkID[3] = 'F';
    wave->header.Format[0] = 'W';
    wave->header.Format[1] = 'A';
    wave->header.Format[2] = 'V';
    wave->header.Format[3] = 'E';
    wave->header.Subchunk1ID[0] = 'f';
    wave->header.Subchunk1ID[1] = 'm';
    wave->header.Subchunk1ID[2] = 't';
    wave->header.Subchunk1ID[3] = ' ';
    wave->header.Subchunk1Size = 16; 
    wave->header.AudioFormat = 1; 
    wave->header.NumChannels = sdata.number_of_channels; 
    wave->header.SampleRate = sdata.sample_rate;
    wave->header.BitsPerSample = sdata.bitrate;
    wave->header.BlockAlign = wave->header.NumChannels * wave->header.BitsPerSample/8;
    wave->header.ByteRate = wave->header.SampleRate * wave->header.NumChannels * wave->header.BitsPerSample/8;
    wave->header.Subchunk2ID[0] = 'd';
    wave->header.Subchunk2ID[1] = 'a';
    wave->header.Subchunk2ID[2] = 't';
    wave->header.Subchunk2ID[3] = 'a';
    wave->header.Subchunk2Size = sdata.left_channel.size()*(sdata.bitrate/8);
    wave->header.ChunkSize = 4 + (8 + wave->header.Subchunk1Size) + (8 + wave->header.Subchunk2Size);
    
    wave->data = new std::vector<int16_t>(sdata.left_channel.begin(), sdata.left_channel.end());
    
}
