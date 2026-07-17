#pragma once

#include <JuceHeader.h>
#include <optional>

class CircularBuffer{
public:
    
    void prepare();
    
    //Process Function v
    std::optional<std::array<float, 80>>  process(float input){
        pushSample(input);
        sampleCounter();
        
    if (sampleCount == 8){
            sampleCount = 0;
        
//        DBG ("FIRING, popping buffer");
            return popBuffer();
        }
    else
        return std::nullopt;
    }
    
    void reset(){
        sampleCount = 0;
    };
    
private:
    
// PRIVATE VARIABLES
    static constexpr int maxBufferSize = 80;
    std::array<float, maxBufferSize> buffer;
    int sampleCount = 0;

// PRIVATE FUNCTIONS
    void pushSample(float input){
        bumpBuffer();
        buffer[0] = input;
    };
    
    void bumpBuffer(){
        for(int i = maxBufferSize - 1; i >0; --i){
            buffer[i] = buffer[i-1];
        }
    };
    
    void sampleCounter(){
        sampleCount ++;
    }
    
    std::array<float, maxBufferSize> popBuffer(){
        return buffer;
    }
    
    
};

/*                  NOTES

 Every new sample passed in needs to override the 1st sample in the array. buffer[0 - 7] need to be the newset samples in the array.

 Sample 79 needs to be deleted before each new sample is added
 
 Every sample needs to be bumped along one value too
 
 Build all functions necesary to the process in the private section and then create a process function which executes all?
 
 
*/
 
