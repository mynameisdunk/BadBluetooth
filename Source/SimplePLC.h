
#pragma once
#include <JuceHeader.h>
#include "DataStructures.h"

class SimplePLC {
    
    public:
    
    EncodedFrame processGood(const EncodedFrame& frame){
    
        EncodedFrame currentFrame = frame;
        burstCounter = 0;
        lastGoodFrame = currentFrame;
        currentFrame.concealmentGain = 1.0f;
        return currentFrame;
        
    }
    
    EncodedFrame processBad(const EncodedFrame& frame){
        
        burstCounter ++;
        lastGoodFrame.concealmentGain = std::pow(burstDecayRate, burstCounter);
        return lastGoodFrame;
    }
    
    
    private:
    
    int burstCounter = 0;
    
    float burstDecayRate = 0.6f;
    
    EncodedFrame lastGoodFrame{};
    EncodedFrame emptyFrame{};
};

