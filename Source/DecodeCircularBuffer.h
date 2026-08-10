/*
  ==============================================================================

    DecodeCircularBuffer.h
    Created: 29 Jul 2026 4:37:04pm
    Author:  Harry Dunk

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "SynthesisFilter.h"
#include "Windowing.h"


class DecodeCircularBuffer {
    
    public:
    
    std::array<float, 8> process(std::array<float, 8> block){
        bumpBuffer();
        
        
        auto matrixedSamples = synthesisFilter.matrixing(block);
        
        for (int i = 0; i < 16; i ++){
            buffer[i] = matrixedSamples[i];
        }
    
        auto windowedSamples = windower.process(synthesisFilter.buildArrayToWindow(buffer));
        
        output.fill( 0.0f);
        for (int j = 0; j < 8; j++){
            for (int i = 0; i < 10; i ++){
                output[j] += windowedSamples[j + (8 * i)];
            }
        }
        
        return output;
        
    }
    
    
    private:
    
    
    void bumpBuffer(){
        for (int i = 159; i > 15; i--){
            buffer[i] = buffer[i - 16];
        }
    }
    
    
    
    std::array<float, 160> buffer{};
    
    std::array<float, 8> output{};
    
    SynthesisFilter synthesisFilter;
    Windower windower;
};
