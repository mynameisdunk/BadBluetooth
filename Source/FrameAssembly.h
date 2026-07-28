
#pragma once

#include <JuceHeader.h>
#include "AnalysisFilter.h"
#include "BitAllocation.h"
#include "ScaleFactoring.h"
#include "SBCParameters.h"


class FrameAssembly{
    public:
    
    
    
    using Frame = std::array<std::array<float, 8>, 16>;
    //                          16 sets of arrays that contain 8 subbands each

// ============================================================
    
    struct EncodedFrame
    {
        FrameAssembly::Frame quantisedSamples;   // 16x8 quantized values
        std::array<int, 8> scaleFactorIndex;       // raw 0-15 index, per subband
        std::array<int, 8> bits;                 // bits allocated per subband

        // header fields - some fixed for now, some will come from SBCParameters later
        
        int sampleRate;
        int nrofBlocks;
        int channelMode;
        int allocationMethod;
        int nrofSubbands;
        int nrofChannels;
        int bitPool;
        
    };
    
// ============================================================
 
    void prepare(/*const SBCParameters& parameters*/){
        
    // This is where any fixed necessary variables are set. Typically sample rate but this plugin will get this information from the frame header so I don't think it needs to be put here??
        
        
    };
    
// ============================================================
    
    void update();
    // This is where the user parameters are updated from - it must be called before process

// ============================================================
    //process function
    std::optional<EncodedFrame> process (std::array<float, 80> samples, const SBCParameters& parameters){
        // return type is either a Full frame or nothing
        // right now though these are not full frames.. These are groups of blocks. The frame needs to contain the header information too.
        
        auto currentBlock = analysisFilter.process(samples);
            // std::array<float, 8>
        frame[frameCount] = currentBlock;
        scaleFactoring.process(currentBlock);
        frameCount++;
        
        // Will need a couple of functions or '=' to use the header information to inform the encoding process.. in fact most of the process will need to know the information stored in the header - the header will be built mostly from parameter choices. Weirdly while the data is needed to set up the process it needs to be passed through transmission to the decode function too so it needs to be used twice in different ways.
        
        if (frameCount ==16)
        {
//            DBG ("frame complete, about to reset");
//            We have processed 16 blocks of audio.. The variable name frame Count is misleading here
            frameCount = 0;
//            DBG ("about to call setBitLevel");
            
            scaleFactors = scaleFactoring.quantise();
            scaleFactorIndex = scaleFactoring.getScaleFactorIndex();
            
            setBitLevel(parameters);
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
            result.scaleFactorIndex = scaleFactorIndex;
            result.bits = bitLevel;
            
            result.sampleRate = parameters.sampleRate;
            result.nrofBlocks = parameters.nrofBlocks;
            result.channelMode = parameters.channelMode;
            result.nrofSubbands = parameters.nrofSubbands;
            result.bitPool = parameters.bitPool;
            
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
    std::array<int, 8> scaleFactorIndex{};
    std::array<int, 8> bitLevel{};
    
// FUNCTIONS
    
// This is where all of the subfunctions need to exists which will when all used together build the frame itself. Then in the process function above there will be (hopefully ) a nice chain which uses these together to create a perfect frame :)
    
    void setBitLevel(SBCParameters parameters){
        bitLevel = bitAllocation.process(scaleFactorIndex, parameters);
    }

// ============================================================
    
    std::optional<Frame> quantiseSamples()
    {
        auto scalefactorConverted = scaleFactoring.getScaleFactor(); // pow(2, index+1), per subband

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
