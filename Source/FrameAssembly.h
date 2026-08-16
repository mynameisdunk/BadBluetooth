
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
        
        scaleFactoring.update(parameters);
        
    };
    void reset();

// ------------------------------------------------------------------------------------
    
    //process function
    std::optional<EncodedFrame> process (std::array<float, 80> samples, const SBCParameters& parameters)
    {
        
        auto currentBlock = analysisFilter.process(samples);

        frame[frameCount] = currentBlock;
        scaleFactoring.process(currentBlock);
        frameCount++;
        
        if (frameCount ==16)
        {
            frameCount = 0;
            EncodedFrame result;
            
            result.sbcParameters.bitPoolResolutionScaling = parameters.bitPoolResolutionScaling;
            result.encodedAudioValues.scaleFactors = scaleFactoring.quantise();
            result.encodedAudioValues.scaleFactorIndex = scaleFactoring.getScaleFactorIndex();
            result.encodedAudioValues.bitLevel = setBitLevel(result, parameters);
            
//            DBG(parameters.bitPool);
            
            quantisedSamples = *quantisation.quantiseSamples(result, frame);

            setEncodedFrameValues(result, parameters);

            scaleFactoring.reset();
            
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
    Frame quantisedSamples{};
    
    
    AnalysisFilter analysisFilter;
    BitAllocation bitAllocation;
    ScaleFactoring scaleFactoring;
    Quantisation quantisation;
};
