
#pragma once
#include <JuceHeader.h>
#include "FrameAssembly.h"
#include "DataStructures.h"


class Reconstruction {
    public:
    
    

    Frame process(const ReconstructionValues& values)
    {
        
        for (int sb = 0; sb < 8; sb++)
        {
            levels[sb] = std::pow(2.0f, static_cast<float>(values.bitLevel[sb])) - 1.0f;
        }
        
        for (int blk = 0; blk < 16; blk++)
        {
            for (int sb = 0; sb < 8; sb++)
            {
                if (levels[sb] > 0.0f)
                {
                    sbSample[blk][sb] = values.scaleFactors[sb] *
                    ((values.quantisedSamples[blk][sb] * 2.0f + 1.0f) / levels[sb] - 1.0f);
                }
                else
                {
                    sbSample[blk][sb] = 0.0f;
                }
            }
        }
        return sbSample;
}
    private:
    
    std::array<float, 8> levels;
    
    
    Frame sbSample;
    
    
};
