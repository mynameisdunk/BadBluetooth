#pragma once
#include <JuceHeader.h>
#include "DataStructures.h"


class GilbertElliot{
    friend class StateMachine;
    public:
    
    void setState(float pathLoss){
       
       calculateRSSI(randomisePathLoss(pathLoss));
//         calculateRSSI(pathLoss);
        if (RSSI > goodRSSI){
            currentState = BurstState::good;}
        
        else if (RSSI < badRSSI){
            currentState = BurstState::fail;}
        
        else {
            calculateX();
            calculateL();
            calculateP();
            calculateR();
            assessStateTransition();
        }
 
    }
    
    BurstState getState(){
        return currentState;
    }
    
    
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
        int burstDepth = 10;
    
    int jitterAmount = 2;
    

    // ENUMERATIONS AND FUNCTIONS
        
        BurstState currentState = BurstState::good;
    
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
//            DBG("bad");
        }
        else {
            if (random.nextFloat() < r)
                currentState = BurstState::good;
//            DBG("good");
        }
    }
    
    // ------------------------------------------------------------------------------------------------
    
    float randomisePathLoss(float pathLoss){
        if (random.nextFloat() > 0.99f){
            float jitter = (random.nextFloat() * 2.0f - 1.0f) * jitterAmount;
            return pathLoss + jitter;
        }
        return pathLoss;
    }
    
    // INSTANCES
        
        StereoEncodedFrame emptyFrame{};
        StereoEncodedFrame lastGoodFrame{};
        
        SimplePLC simplePLC;
        
        juce::Random random;
    
};

/*
 
 if (splitPathLoss){
     output[0] = lossConcealmentL.process(stereoDecoder.getNextSample(0));
     output[1] = lossConcealmentR.process(stereoDecoder.getNextSample(1));
 }
 
 else {
     output[0] = lossConcealment.process(stereoDecoder.getNextSample(0));
     output[1] = lossConcealment.process(stereoDecoder.getNextSample(1));
 }
 */
