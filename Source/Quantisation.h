
#pragma once
#include "DataStructures.h"

class Quantisation{
    
    public:
    
    std::optional<Frame> quantiseSamples(EncodedFrame& result, const Frame& frame)
    {
        auto scalefactorConverted = result.encodedAudioValues.scaleFactors;
        auto bitLevel = result.encodedAudioValues.bitLevel;

        std::array<float, 8> levels {};
        for (int sb = 0; sb < 8; sb++)
            levels[sb] = std::pow (2.0f, static_cast<float> (bitLevel[sb])) - 1.0f;

        for (int blk = 0; blk < 16; blk++)
        {
            for (int sb = 0; sb < 8; sb++)
            {
                if (levels[sb] <= 0.0f)
                {
                    quantisedSamples[blk][sb] = 0.0f; // 0 bits allocated - nothing to encode for this subband
                    continue;
                }

                float normalized = (frame[blk][sb] / scalefactorConverted[sb] + 1.0f) * levels[sb] / 2.0f;
                quantisedSamples[blk][sb] = std::floor (normalized);
            }
        }

        return quantisedSamples;
    }
    
    
    
    private:
    
    Frame quantisedSamples{};
    
};
