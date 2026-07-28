/*
  ==============================================================================

    h.h
    Created: 28 Jul 2026 5:52:49pm
    Author:  Harry Dunk

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "FrameAssembly.h"
#include "SBCParameters.h"
#include "ScaleFactoring.h"
#include "BitAllocation.h"
#include "Reconstruction.h"

class Decoder{
    public:
    
    void process(const FrameAssembly::EncodedFrame& frame){
        
        rawFrame = frame;
        setParameters(frame);
        scaleFactorIndexes = frame.scaleFactorIndex;
        /*
         PROCESS ORDER
         
         Scale Factoring
         - Calculate scale factor values from scaleFactorIndex
         - scalefactor[ch][sb] = pow(2.0,(scaleFactorIndex[ch][sb]+1)).
         
         Bit Allocation
         - This is the exact same as done by the encoding process
         
         Reconstruction of Subband Samples
         - outlined in the spec
         
         Synthesis Filter
         
         
         */
        
        reconstructionValues.scaleFactors = scaleFactoring.calculateScaleFactors(scaleFactorIndexes);
        reconstructionValues.bitLevel = bitAllocation.process(scaleFactorIndexes, sbcParameters);
        reconstructionValues.quantisedSamples = rawFrame.quantisedSamples;
        
        processedFrame = reconstruction.process(reconstructionValues);
        
        
    };
    
    
    
    private:
    
    void setParameters(const FrameAssembly::EncodedFrame frame){
        sbcParameters.sampleRate = frame.sampleRate;
        sbcParameters.nrofSubbands = frame.nrofSubbands;
        sbcParameters.nrofBlocks = frame.nrofBlocks;
        sbcParameters.nrofChannels = frame.nrofChannels;
        sbcParameters.channelMode = frame.channelMode;
        sbcParameters.allocationMethod = frame.allocationMethod;
        sbcParameters.bitPool = frame.bitPool;
    }
    
    
    
    // PRIVATE VARIABLES
    std::array<int, 8> scaleFactorIndexes{};

    
    // INSTANCES
    
    SBCParameters sbcParameters;
    ScaleFactoring scaleFactoring;
    BitAllocation bitAllocation;
    Reconstruction reconstruction;
    FrameAssembly::EncodedFrame rawFrame;
    FrameAssembly::Frame processedFrame;
    
    ReconstructionValues reconstructionValues;
    
};
