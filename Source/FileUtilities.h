#pragma once
#include <cstdint>
#include <fstream>
#include <vector>

const int INT24_MAX = 8388607;
// For 24-bit samples, minimal functionality
#pragma pack(push)
#pragma pack(1)
struct int24_t {
    uint8_t bytes[3];
    int24_t()
    {
        bytes[0] = 0;
        bytes[1] = 0;
        bytes[2] = 0;
    };
    int24_t(const double& value)
    {
        int int_value{static_cast<int32_t>(value)};
        //if(int_value < 0)
        //{
            bytes[0] = (int_value) & 0xFF;
	        bytes[1] = (int_value >> 8) & 0xFF;
            bytes[2] = (int_value >> 16) & 0xFF;
        //}
        //else
        //{
            //bytes[0] = (int_value) & 0xFF;
	        //bytes[1] = (int_value >> 8) & 0xFF;
          //  bytes[2] = (int_value >> 16) & 0xFF;
        //}
        
    };
#pragma pack(pop)

    operator int() const
    {
        if ( bytes[2] & 0x80 ) // Is this a negative?  Then we need to singn extend.
        {
            return (0xff << 24) | (bytes[2] << 16) | (bytes[1] << 8) | (bytes[0] << 0);
        }
        else
        {
            return (bytes[2] << 16) | (bytes[1] << 8) | (bytes[0] << 0);
        }
    };

    operator double() const
    {
        return static_cast<double>(this->operator int());
    };
};

int16_t readS16(std::ifstream&);
void writeS16(std::ofstream&, int16_t);
    
int24_t readS24(std::ifstream&);
void writeS24(std::ofstream&, int24_t);

int32_t readS32(std::ifstream&);
void writeS32(std::ofstream&, int32_t);

