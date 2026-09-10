#pragma once

#include <JuceHeader.h>
#include <optional>

class EncodeCircularBuffer{
public:
    
    static constexpr int numChannels = 2;
    static constexpr int maxBufferSize = 80;
    
    
    void prepare();
    
    //Process Function v
    
    std::optional<std::array<std::array<float, maxBufferSize>, numChannels>>
    // optional containing <array of type <80 sample float array>, two of them>
    process(std::array<float, numChannels> input){
        
        for (int ch = 0; ch < numChannels; ch++)
            
            pushSample(input[ch], ch);
            sampleCounter();
        
        if (sampleCount == 8){
                sampleCount = 0;
        
//               DBG ("FIRING, popping buffer");
            return buffer;
            }
        else
            return std::nullopt;
        }
        
        void reset(){
            sampleCount = 0;
    };
    
private:
    
// PRIVATE VARIABLES
    
//    std::array<float, maxBufferSize> buffer{};
    std::array<std::array<float, maxBufferSize>, numChannels> buffer{};
    int sampleCount = 0;

// PRIVATE FUNCTIONS
    void pushSample(float input, int ch){
        bumpBuffer(ch);
        buffer[ch][0] = input;
    };
    
    void bumpBuffer(int ch){
        for(int i = maxBufferSize - 1; i >0; --i){
            buffer[ch][i] = buffer[ch][i-1];
        }
    };
    
    void sampleCounter(){
        sampleCount ++;
    }
//    
//    std::array<float, maxBufferSize> popBuffer(){
//        return buffer;
//    }
    
    
};

/*                  NOTES

 Every new sample passed in needs to override the 1st sample in the array. buffer[0 - 7] need to be the newset samples in the array.

 Sample 79 needs to be deleted before each new sample is added
 
 Every sample needs to be bumped along one value too
 
 Build all functions necesary to the process in the private section and then create a process function which executes all?
 
 
*/
 
