
#pragma once
#include <JuceHeader.h>


class LossCircularBuffer{
    
    public:
    
    using Buffer = std::array<float, 390>;
    
    void pushSample(float nextSample){
        buffer[writeIndex] = nextSample;
        
        writeIndex ++;
        if (writeIndex + 1 == size){
                    writeIndex = 0;
                }

    }
    
    void copyToLinear() {
        pitchBuffer = buffer;
    }
    
    void reset(){
        buffer.fill(0.0f);
    }
    
    
    private:
    
    Buffer buffer;
    Buffer pitchBuffer;
    
    int writeIndex = 0;
    int size = 390;
    
    
    
    
    
};
