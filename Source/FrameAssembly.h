
#pragma once

#include <JuceHeader.h>
#include "AnalysisFilter.h"
#include "BitAllocation.h"
#include "ScaleFactoring.h"

class FrameAssembly{
    public:
    
    using Frame = std::array<std::array<float, 8>, 16>;
    //                          16 sets of arrays that contain 8 subbands each

// ============================================================
    
    struct EncodedFrame
    {
        FrameAssembly::Frame quantisedSamples;   // 16x8 quantized values
        std::array<float, 8> scaleFactors;       // raw 0-15 index, per subband
        std::array<int, 8> bits;                 // bits allocated per subband

        // header fields - some fixed for now, some will come from SBCParameters later
        int samplingFrequencyIndex = 3;   // 48kHz for now
        int allocationMethod = 0;         // LOUDNESS/SNR
        int bitpool = 52;
    };
    
    void prepare();
    
// ============================================================
    
    void update();

// ============================================================
    //process function
    std::optional<EncodedFrame> process (std::array<float, 80> samples){
        // return type is either a Full frame or nothing
        // right now though these are not full frames.. These are groups of blocks. The frame needs to contain the header information too.
        
        auto currentBlock = analysisFilter.process(samples);
        frame[frameCount] = currentBlock;
        scaleFactors = scaleFactoring.process(currentBlock);
        frameCount++;
        
        
        
        if (frameCount ==16)
        {
//            DBG ("frame complete, about to reset");
            frameCount = 0;
//            DBG ("about to call setBitLevel");
            setBitLevel();
//            DBG ("setBitLevel done, about to call quantiseSamples");
            quantiseSamples();
//            DBG ("quantiseSamples done");
            
//            for (int blk = 0; blk < 16; blk++)
//               {
//                   juce::String s;
//                   for (int sb = 0; sb < 8; sb++)
//                       s << frame[blk][sb] << " ";
//                   DBG ("block " << blk << ": " << s);
//               }
            
            EncodedFrame result;
            result.quantisedSamples = quantisedSamples;
            result.scaleFactors = scaleFactors;
            result.bits = bitLevel;
            
            return result;
        }
        
        return std::nullopt;
        
    };
// ============================================================
    void reset();
        
// ============================================================
    
    private:
    
// VARIABLES
    int frameCount = 0;
    
    float block[8];
    
    std::array<float, 8> scaleFactors{};
    std::array<int, 8> bitLevel{};
    
// FUNCTIONS
    
// This is where all of the subfunctions need to exists which will when all used together build the frame itself. Then in the process function above there will be (hopefully ) a nice chain which uses these together to create a perfect frame :)
    
    void setBitLevel(){
        bitLevel = bitAllocation.process(scaleFactors);
    }

// ============================================================
    
    std::optional<Frame> quantiseSamples()
    {
        auto scalefactorConverted = scaleFactoring.getScalefactor(); // pow(2, index+1), per subband

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

// ============================================================
    
// INSTANCES
    Frame frame{};
    Frame quantisedSamples{};
    
    
    AnalysisFilter analysisFilter;
    BitAllocation bitAllocation;
    ScaleFactoring scaleFactoring;
};
