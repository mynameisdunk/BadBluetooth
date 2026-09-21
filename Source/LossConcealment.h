
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

/*
 This class is going to be a weird one to work on because the stereoDecoder.getNextSample is going to feed the loss circular buffer. Then when this class is given the bad state the concealment will begin. So the gilbert elliot logic needs to exist here too.. At this point I need to create a class which means that the next sample output is coming from the temporary non circular buffer (pitch buffer).
 
 All good samples fed into circular buffer (safety buffer)
 
 As soon as state turns bad the entire safety buffer is copied into a linear array.
 
 Then this pitch buffer is looped until the state becomes good again.
 
 When the state becomes good again the samples are read from the stereoDecoder once again.
 */
