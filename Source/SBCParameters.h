
#pragma once

struct SBCParameters{
    
    int sampleRate;
    int nrofSubbands = 8;
    int nrofBlocks = 16;
    int nrofChannels = 2;
    int channelMode;
    int allocationMethod = 0;
    int bitPool = 32;
};

struct ReconstructionValues{
    
    std::array<std::array<float, 8>, 16> quantisedSamples{};
    std::array<float, 8> scaleFactors{};
    std::array<int, 8> bitLevel{};
    
};

