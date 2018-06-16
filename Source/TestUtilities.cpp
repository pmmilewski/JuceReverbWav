#include "TestUtilities.h"
#include <cstdint>
// For test purposes, no parameters
SoundData* createImpulseSD()
{
    SoundData* signal = new SoundData;
    
    signal->number_of_channels = 1;
    signal->sample_rate = 44100;
    signal->bitrate = 16;
    
    signal->left_channel.push_back(1);
    signal->left_channel.resize(signal->sample_rate*5, 0);

    signal->initial_size = signal->left_channel.size();
    
    return signal;
}

void fft(CArray& x)
{
    const size_t N = x.size();
    if(N <=1 ) return;

    CArray even = x[std::slice(0, N/2, 2)];
    CArray odd = x[std::slice(1, N/2, 2)];

    fft(even);
    fft(odd);

    
    for(size_t i = 0; i < N/2; ++i)
    {
        Complex t = std::polar(1.0, -2* M_PI * i / N) * odd[i];
        x[i] = even[i] + t;
        x[i+N/2] = even[i] - t;
    }
    
}

void ifft(CArray& x)
{
    x = x.apply(std::conj);
    
    fft(x);
    
    x = x.apply(std::conj);

    x /= x.size();
}

CArray* getCArrayFromSoundDataChannel(const SoundData* sdata, int channel_number = 0)
{
    CArray* complex_samples = nullptr;
    ///
    int closest_2power = static_cast<int>(std::log2(sdata->initial_size))+1;
    size_t new_size = static_cast<int>(pow(2,closest_2power));
    ///
    switch(channel_number)
    {
        case 0:
        {
            complex_samples = new CArray(Complex(0), new_size);
            for(size_t i = 0; i < sdata->initial_size; i++)
            {
                (*complex_samples)[i] = Complex(sdata->left_channel[i]);
            }
            break;
        }
        case 1:
        {
            complex_samples = new CArray(Complex(0), new_size);
            for(size_t i = 0; i < sdata->initial_size; i++)
            {
                (*complex_samples)[i] = Complex(sdata->right_channel[i]);
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return complex_samples;

}

void setChannelFromCArray(SoundData* sdata, const CArray* complex_samples, int channel_number = 0)
{
    switch(channel_number)
    {
        case 0:
        {
            size_t size = sdata->left_channel.size();
            for(size_t i = 0; i < size; i++)
            {
                sdata->left_channel[i] = ((*complex_samples)[i]).real();
            }
            break;
        }
        case 1:
        {
            size_t size = sdata->right_channel.size();
            for(size_t i = 0; i < size; i++)
            {
                sdata->right_channel[i] = ((*complex_samples)[i]).real();
            }
            break;
        }
        default:
        {
            break;
        }
    }
}
