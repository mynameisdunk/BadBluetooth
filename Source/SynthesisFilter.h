/*
  ==============================================================================

    SynthesisFilter.h
    Created: 28 Jul 2026 7:53:31pm
    Author:  Harry Dunk

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SynthesisFilter{
    public:
    

    std::array<float, 16> matrixing(std::array<float, 8> block){
        
        for (int k = 0; k < 16; k++){
            matrixedSamples[k] = 0.0f;
            for (int i = 0; i < 8; i ++){
                N[k][i] = cos((i + 0.5) * (k + 4) * (M_PI / 8));
                matrixedSamples[k] += (N[k][i] * block[i]);
            }
        }
        return matrixedSamples;
    }
    
    std::array<float, 80> buildArrayToWindow(std::array<float, 160> buffer){
        
        for (int i = 0; i < 5; i ++){
            for (int j = 0; j < 8; j++){
                windowingArray[(i * 16) + j] = buffer[(i * 32) + j];
                windowingArray[(i * 16) + 8 + j] = buffer[(i * 32) + 24 + j];
            }
        }
        return windowingArray;
    }
    
    
    private:
    
    std::array<std::array<float, 8>, 16> N{};
    
    std::array<float, 16> matrixedSamples{};
    
    std::array<float, 80> windowingArray{};

    std::array<float, 16> output{};
    
};
