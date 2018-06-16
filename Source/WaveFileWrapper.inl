// Sample number manipulation
    template <typename T>
    void addSamples(const int &n)
    {
        auto new_size = wave->header.Subchunk2Size += n*sizeof(T);
        std::vector<T> *data = static_cast<std::vector<T>*>(wave->data);
        data->resize(new_size);
    };
    
    /// RW helper functions templates
    /// Refactor of read/write sample in different sizes
    template <int T>
    void readSamplesFromFile(std::ifstream& fs)
    {
        auto sample{ 0 };
        switch(T)
        {
            case 8:
            {
                uint8_t byte;
                auto *data = new std::vector<uint8_t>(0);
                auto sample{0};
                while (sample < wave->header.Subchunk2Size)
                {
                    fs.seekg(sizeof(wave->header)+sample);
                    fs.read(reinterpret_cast<char*>(&byte), 1);
                    data->push_back(byte);
                    sample++;
                }
                wave->data = data;
                break;
            }
            case 16:
            {
                auto *data = new std::vector<int16_t>(0);
                auto sample{0};
                while (sample < wave->header.Subchunk2Size/2)
                {
                    fs.seekg(sizeof(wave->header)+(sample*2));
                    data->push_back(readS16(fs));
                    sample++;
                }
                wave->data = data;
                break;
            }
            case 24:
            {
                auto *data = new std::vector<int24_t>(0);
                auto sample{0};
                while (sample < wave->header.Subchunk2Size/3)
                {
                    fs.seekg(sizeof(wave->header)+(sample*3));
                    data->push_back(readS24(fs));
                    sample++;
                }
                wave->data = data;
                break;
            }
            case 32:
            {
                auto *data = new std::vector<int32_t>(0);
                auto sample{0};
                while (sample < wave->header.Subchunk2Size/4)
                {
                    fs.seekg(sizeof(wave->header)+(sample*4));
                    data->push_back(readS32(fs));
                    sample++;
                }
                wave->data = data;
                break;
            }
            default:
                std::cout << "Something is wrong with BitPerSample" << std::endl;
        };
        
    };

    template <int T>
    void writeSamplesToFile(std::ofstream& fs)
    {
        auto sample{ 0 };
        switch (T)
        {
            case 8:
            {
                auto* data = (std::vector<uint8_t>*)wave->data;
                while (sample < wave->header.Subchunk2Size)
                {
                    fs.write(reinterpret_cast<char*>(&(*data)[sample]), 1);
                    sample++;
                }
                break;
            }
            case 16:
            {
                auto* data = (std::vector<int16_t>*)wave->data;
                while (sample < wave->header.Subchunk2Size / 2)
                {
                    writeS16(fs, (*data)[sample]);
                    sample++;
                }
                break;
            }
            case 24:
            {
                auto* data = (std::vector<int24_t>*)wave->data;
                while (sample < wave->header.Subchunk2Size / 3)
                {
                    writeS24(fs, (*data)[sample]);
                    sample++;
                }
                break;
            }
            case 32:
            {
                auto* data = (std::vector<int32_t>*)wave->data;
                while (sample < wave->header.Subchunk2Size / 4)
                {
                    writeS32(fs, (*data)[sample]);
                    sample++;
                }
                break;
            }
            default:
            {
                std::cout << "Something is not right with BitsPerSample" << std::endl;
                break;
            }
        };
    };