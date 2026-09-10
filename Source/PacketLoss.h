
#pragma once
#include <JuceHeader.h>
#include "DataStructures.h"
#include "SimplePLC.h"

class PacketLoss {
    public:
    
    EncodedFrame process(const EncodedFrame& frame, float pathLoss){
        
        calculateRSSI(pathLoss);
        EncodedFrame currentFrame = frame;
        
        if (RSSI > goodRSSI){
            return currentFrame;}
        
        else if (RSSI < badRSSI){
            return emptyFrame;}
        
        else {
            calculateX();
            calculateL();
            calculateP();
            calculateR();
            assessStateTransition();
        }
    
        if (currentType == PLCType::simple){
            
            if (currentState == BurstState::good){
                return simplePLC.processGood(currentFrame);
            }
            else {
                return simplePLC.processBad(currentFrame);
            }
        }
        
        else {
            return currentFrame;
            // TOTALLY WRONG
        }
    }
    
    

    // ------------------------------------------------------------------------------------------------
    private:
    
// VARIABLES
    
    const int Tx = 4;
    float RSSI = 0.0f;
    
    const float goodRSSI = -60.0f;
    const float badRSSI = -82.0f;
    float range = std::abs(badRSSI - goodRSSI);
    
    float x = 0.0f;
    float L = 0.0f;
    float p = 0.0f;
    float r = 0.0f;
   
    float knee = 6.0f;
    float burstDepth = 1.0f;

// ENUMERATIONS AND FUNCTIONS
    
    enum class BurstState {good, bad};
    BurstState currentState = BurstState::good;
    
    enum class PLCType {simple, vocal};
    PLCType currentType = PLCType::simple;
    
    // move this into pluginProcessor somehow???????
    // I need this to be known before getting into the sub classes because one concealment type relies on reading frames and another needs decoded samples stored into a pitchBuffer....
    
    
    // ------------------------------------------------------------------------------------------------
    
    void calculateX(){
        if (RSSI > goodRSSI){
            x = 0.0f;}
        
        else if (RSSI < badRSSI){
            x = 1.0f;}
        
        else {
            float fraction = (goodRSSI - RSSI) / range;
            float centred = fraction - 0.5f;
            float t = knee * centred;
            x = 1.0f / (1.0f + std::exp(-t));
        }
    }
    
    // ------------------------------------------------------------------------------------------------
    
    void calculateL(){
        float minL = x / (1.0f - x);
        if (minL < 1.0f) minL = 1.0f;      // r = 1/L can't exceed 1 either, so L can't go below 1
        L = minL * (1.0f + burstDepth);
    }
    
    // ------------------------------------------------------------------------------------------------
    
    void calculateP(){
        p = x / (L * (1 - x));
    }
    
    // ------------------------------------------------------------------------------------------------
    
    void calculateR(){
        r = 1 / L;
    }
    
    // ------------------------------------------------------------------------------------------------
    
    void calculateRSSI(float pathLoss){
        RSSI = Tx - pathLoss;
    }
    
    // ------------------------------------------------------------------------------------------------
    
    void assessStateTransition(){
        if (currentState == BurstState::good){
            if (random.nextFloat() < p)
                currentState = BurstState::bad;
        }
        else {
            if (random.nextFloat() < r)
                currentState = BurstState::good;
        }
    }
    
    // ------------------------------------------------------------------------------------------------

// INSTANCES
    
    EncodedFrame emptyFrame{};
    EncodedFrame lastGoodFrame{};
    
    SimplePLC simplePLC;
    
    juce::Random random;
    
};


// this class needs to be used as the starting point for all packetloss situations - BASICALLY this class is only going to be responsible for deciding whether or not there is packet loss taking place. Then I will use the state machine output to trigger the appropriate loss concealment plan.

// So - the circular buffer method (ripped from VoCAL) will be called if the state is BAD through the loss concealment class.

// Later down the line I can include types of loss concealment as a parameter. To begin with this may literally be type 1 (vocal) and type 0 (none *as above is currently*) but this can all be figured out along the way

// The current methods used to conceal the packets need to be put into there own class called something like "Simple PLC"

// UPDATE #1                                                                                                    I have added in the simplePLC class so now when the state==good / bad it is using this class to return values rather than doing the processing within this class.

// To turn this into a parameter I will need to include another argument which could be done using a number system for the parameters where each method is assigned a number. Then this can be easily refactored any time I want to add a new method of PLC
