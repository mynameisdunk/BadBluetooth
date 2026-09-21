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
#include "LossConcealment.h"

class Decoder{
    public:

    void prepare(juce::dsp::ProcessSpec& spec){
        // write a class which prepares all known and needed values here.. Probably using a juce spec reference is the best way
        numChannels = spec.numChannels;
        
    }
    
    void process(const StereoEncodedFrame& frame){

        for (int ch = 0; ch < numChannels; ch++){
            stereoRawFrame[ch] = frame[ch];
            //            setParameters(frame[ch], ch);
            //            sbcParameters = frame[ch].sbcParameters;
            scaleFactorIndexes = frame[ch].encodedAudioValues.scaleFactorIndex;
            outputGain[ch] = frame[ch].concealmentGain;
            
            
            reconstructionValues[ch].scaleFactors = scaleFactoring.calculateScaleFactors(scaleFactorIndexes);
            reconstructionValues[ch].bitLevel = bitAllocation.process(scaleFactorIndexes, frame[ch].sbcParameters);
            reconstructionValues[ch].quantisedSamples = stereoRawFrame[ch].encodedAudioValues.quantisedSamples;
            reconstructionValues[ch].scaleFactorIndex = scaleFactorIndexes;
            
            processedStereoFrame[ch] = reconstruction.process(reconstructionValues[ch]);

            for(int blc = 0; blc < 16; blc++){
                temporary = processedStereoFrame[ch][blc];
                currentBlock = decodeBuffer[ch].process(temporary);
                outputQueue[ch].push(currentBlock);
          
            }
        }
    };

    float getNextSample(int ch){

        float output = outputQueue[ch].pop() * outputGain[ch];
        // pass output to loss concealment buffer
        lossConcealment[ch].process(output);
        return output;

    }

    void reset(){
        
        for (int ch = 0; ch < numChannels; ch++){
            outputQueue[ch].reset();
            decodeBuffer[ch].reset();
            lossConcealment[ch].reset();
        }

    }


    private:

    
    std::array<float, 8> popSamples(std::array<float, 8> output){
        // dead code?

        for (int i = 0; i < 8; i++){
            output[i];
        }

        return output;
    }



    // PRIVATE VARIABLES
    std::array<int, 8> scaleFactorIndexes{};
    std::array<float, 8> temporary{};
    std::array<float, 8> currentBlock{};
    std::array<float, 8> output{};
    
    int numChannels = 2;

    StereoEncodedFrame stereoRawFrame{};
    Frame processedFrame{};
    StereoFrame processedStereoFrame{};


    std::array<float, 2> outputGain{1.0f, 1.0f};

    // INSTANCES

    ScaleFactoring scaleFactoring;
    BitAllocation bitAllocation;
    Reconstruction reconstruction;

    std::array<DecodeCircularBuffer, 2> decodeBuffer{};
    std::array<ReconstructionValues, 2> reconstructionValues{};
    std::array<OutputQueue, 2> outputQueue{};
    std::array<LossConcealment, 2> lossConcealment;


// DBG variables
//    int pushedCounter = 0;
//    int poppedCounter = 0;


};
