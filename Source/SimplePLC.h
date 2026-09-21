
#pragma once
#include <JuceHeader.h>
#include "DataStructures.h"

class SimplePLC {
    
    public:
    
    StereoEncodedFrame processGood(const StereoEncodedFrame& frame){
        
        StereoEncodedFrame currentFrame = frame;
        burstCounter = 0;
        
        for (int ch = 0; ch < 2; ch++){
            
            lastGoodFrame[ch] = currentFrame[ch];
            currentFrame[ch].concealmentGain = 1.0f;
        }
        return currentFrame;
        
    }
    
    StereoEncodedFrame processBad(const StereoEncodedFrame& frame){
        
        burstCounter ++;
        for (int ch = 0; ch < 2; ch++){
            lastGoodFrame[ch].concealmentGain = std::pow(burstDecayRate, burstCounter);
        }
        return lastGoodFrame;
    }
    
    
    private:
    
    int burstCounter = 0;
    
    float burstDecayRate = 0.6f;
    
    StereoEncodedFrame lastGoodFrame{};
    StereoEncodedFrame emptyFrame{};
};

