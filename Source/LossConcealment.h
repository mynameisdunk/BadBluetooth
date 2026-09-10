
#pragma once
#include <JuceHeader.h>
#include "LossCircularBuffer.h"

class LossConcealment{
    
    public:
    
    void prepare();
    void update();
    void process(float nextSample){
        
        buffer.pushSample(nextSample);
        
        
    }
    void reset(){
        buffer.reset();
    }
    
    
    
    
    private:
    
    
    LossCircularBuffer buffer;
    
    
};
