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
#include "DataStructures.h"
#include "ScaleFactoring.h"
#include "BitAllocation.h"
#include "Reconstruction.h"
#include "SynthesisFilter.h"
#include "DecodeCircularBuffer.h"
#include "OutputQueue.h"


class Decoder{
    public:
    
    void process(const EncodedFrame& frame){
        
        rawFrame = frame;
        setParameters(frame);
        scaleFactorIndexes = frame.encodedAudioValues.scaleFactorIndex;
        /*
         PROCESS ORDER
         
    DONE Scale Factoring
         - Calculate scale factor values from scaleFactorIndex
         - scalefactor[ch][sb] = pow(2.0,(scaleFactorIndex[ch][sb]+1)).
         
    DONE Bit Allocation
         - This is the exact same as done by the encoding process
         
    DONE Reconstruction of Subband Samples
         - outlined in the spec
         
         Synthesis Filter
         
         
         */
        
        reconstructionValues.scaleFactors = scaleFactoring.calculateScaleFactors(scaleFactorIndexes);
        reconstructionValues.bitLevel = bitAllocation.process(scaleFactorIndexes, sbcParameters);
        reconstructionValues.quantisedSamples = rawFrame.encodedAudioValues.quantisedSamples;
        
        processedFrame = reconstruction.process(reconstructionValues);
        
        for(int blc = 0; blc < 16; blc++){
            temporary = processedFrame[blc];
            currentBlock = decodeBuffer.process(temporary);
            outputQueue.push(currentBlock);
        }
        
        
//        DBG("PUSHED total so far: " << (pushedCounter += 128));
//        DBG("POPPED total so far: " << (poppedCounter));
        
        

    };
    
    float getNextSample(){
        

      poppedCounter += 1;
        
        return outputQueue.pop();
        
        
    }
    
    
    private:
    
    void setParameters(const EncodedFrame& frame){
        sbcParameters.sampleRate = frame.sbcParameters.sampleRate;
        sbcParameters.nrofSubbands = frame.sbcParameters.nrofSubbands;
        sbcParameters.nrofBlocks = frame.sbcParameters.nrofBlocks;
        sbcParameters.nrofChannels = frame.sbcParameters.nrofChannels;
        sbcParameters.channelMode = frame.sbcParameters.channelMode;
        sbcParameters.allocationMethod = frame.sbcParameters.allocationMethod;
        sbcParameters.bitPool = frame.sbcParameters.bitPool;
    }
    
    std::array<float, 8> popSamples(std::array<float, 8> output){
        
        for (int i = 0; i < 8; i++){
            output[i]; 
        }
        
        return output;
    }
    
    
    
    // PRIVATE VARIABLES
    std::array<int, 8> scaleFactorIndexes{};
    std::array<float, 8> temporary{};
    std::array<float, 8> currentBlock{};
    
    EncodedFrame rawFrame{};
    Frame processedFrame{};
    std::array<float, 8> output{};
    
    // INSTANCES
    
    SBCParameters sbcParameters;
    ScaleFactoring scaleFactoring;
    BitAllocation bitAllocation;
    Reconstruction reconstruction;
    
    DecodeCircularBuffer decodeBuffer;
    
    ReconstructionValues reconstructionValues;
    
    OutputQueue outputQueue;
    
    
// DBG variables
    int pushedCounter = 0;
    int poppedCounter = 0;
    
    
};
