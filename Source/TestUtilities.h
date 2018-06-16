#pragma once
#define _USE_MATH_DEFINES

#include "WaveFileWrapper.h"

#include <complex>
#include <iostream>
#include <valarray>
#include <math.h>

using Complex = std::complex<double>;
using CArray = std::valarray<Complex>;

SoundData* createImpulseSD();

void fft(CArray&);
void ifft(CArray&);

CArray* getCArrayFromSoundDataChannel(const SoundData*, int);
void setChannelFromCArray(SoundData*, const CArray*, int);