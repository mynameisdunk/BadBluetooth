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


/*
 Step 1
 - Input 8 new subband samples (1 block)
 - for i = 0 to i = 7 do
    S[i] = next subband sample
 
 Step 2
 - Shifting
 - for i = 159 to i = 16 do
    V[i] = V[i - 16]
 
 Step 3
 - Matrixing
 - for k = 0 to k = 15 do
    for i = 0 to i = 7 do
        V[k] = sum(N[k][i] * S[i])
    where N[k][i] = cos((i + 0.5) * (k + 4) * (pi / 8))
 
 Step 4
 - Build an 80 values vector U
 - for i = 0 to i = 4 do
    for j = 0 to j = 7 do
        U[(i * 16) + j] = V[(i * 32) + j]
        U[(i * 16) + 8 + j] = V[(i * 32) + 24 + j]
 
 Step 5
 - Window by 80 coeeficients
 - Produce Vector W
 - for i = 0 to i = 79 do
    W[i] = U[i] * D[i] (D[i] : filter coeffs table 12 - 24 multiplied by -8)
 
 Step 6
 - Calculate 8 audio samples
 - for j = 0 to j = 7 do
    for i = 0 to i = 9 do
        X[j] = sum(W[j + (8 * i)])
 Step 7
 - Output 8 reconstructed Audio Samples
 - for i = 0 to i = 7 do
    next outupt audio sample = X[i]
 
 
 
 */


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
