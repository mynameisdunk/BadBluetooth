
#pragma once

#include <JuceHeader.h>
#include "AnalysisFilter.h"
#include "BitAllocation.h"
#include "ScaleFactoring.h"
#include "DataStructures.h"
#include "Quantisation.h"


class FrameAssembly{
    public:
 
    void prepare();
    void update(SBCParameters& parameters){
        for (int ch = 0; ch < 2; ch++){
            scaleFactoring[ch].update(parameters);
        }
    };
    void reset();

// ------------------------------------------------------------------------------------
    
    //process function
    std::optional<StereoEncodedFrame> process (const StereoBlock& samples, const SBCParameters& parameters)
    {
        
        for (int ch = 0; ch < 2; ch++){
            
            auto currentBlock = analysisFilter.process((*samples)[ch]);
            stereoFrame[ch][frameCount] = currentBlock;
            scaleFactoring[ch].process(currentBlock);
            
        }
            frameCount++;
            
            if (frameCount ==16)
            {
                frameCount = 0;
                StereoEncodedFrame result;
                
                for(int ch = 0; ch < 2; ch++){
                    
                    result[ch].sbcParameters.bitPoolResolutionScaling = parameters.bitPoolResolutionScaling;
                    result[ch].encodedAudioValues.scaleFactors = scaleFactoring[ch].quantise();
                    result[ch].encodedAudioValues.scaleFactorIndex = scaleFactoring[ch].getScaleFactorIndex();
                    result[ch].encodedAudioValues.bitLevel = setBitLevel(result[ch], parameters);
                    
                    //            DBG(parameters.bitPool);
                    
                    quantisedSamples = *quantisation.quantiseSamples(result[ch], stereoFrame[ch]);
                    
                    setEncodedFrameValues(result[ch], parameters);
                    
                    scaleFactoring[ch].reset();
                }
                    return result;
            }
        return std::nullopt;
        
    };
// ------------------------------------------------------------------------------------

    private:
    
// VARIABLES
    int frameCount = 0;
    std::array<int, 8> scaleFactorIndex{};

    
// FUNCTIONS
    
    std::array<int, 8> setBitLevel(EncodedFrame& result, const SBCParameters& parameters)
    {
        auto bitLevel = bitAllocation.process(result.encodedAudioValues.scaleFactorIndex, parameters);

        return bitLevel;
    }

// ------------------------------------------------------------------------------------
     
    void setEncodedFrameValues(EncodedFrame& result, const SBCParameters& parameters)
    {
        result.encodedAudioValues.quantisedSamples = quantisedSamples;
        
        result.sbcParameters.sampleRate = parameters.sampleRate;
        result.sbcParameters.nrofBlocks = parameters.nrofBlocks;
        result.sbcParameters.channelMode = parameters.channelMode;
        result.sbcParameters.nrofSubbands = parameters.nrofSubbands;
        result.sbcParameters.bitPool = parameters.bitPool;
    }
    
// ------------------------------------------------------------------------------------
    
// INSTANCES
    Frame frame{};
    StereoFrame stereoFrame{};
    Frame quantisedSamples{};
    
    AnalysisFilter analysisFilter;
    BitAllocation bitAllocation;
    Quantisation quantisation;
    
    std::array<ScaleFactoring, 2> scaleFactoring;
};



