
#pragma once
#include <JuceHeader.h>
#include "DataStructures.h"
#include "SimplePLC.h"
#include "GilbertElliot.h"

class PacketLoss {
    public:
    
    StereoEncodedFrame process(const StereoEncodedFrame& frame, BurstState currentState){
        
        StereoEncodedFrame currentFrame = frame;
        
        switch (currentState){
            case BurstState::good:
            {
                return simplePLC.processGood(currentFrame);
            }
                break;
                
            case BurstState::bad:
            {
                return simplePLC.processBad(currentFrame);
            };
                break;
                
            case BurstState::fail:
            {
                return emptyFrame;
            };
                break;
                
            default:
                
            {return emptyFrame;};
        }
    }
    
    

    // ------------------------------------------------------------------------------------------------
    private:

// INSTANCES
    
    StereoEncodedFrame emptyFrame{};
    StereoEncodedFrame lastGoodFrame{};
    
    SimplePLC simplePLC;
    
};

