
#pragma once
#include <JuceHeader.h>
#include "LossCircularBuffer.h"
#include "DataStructures.h"

class LossConcealment{
    
    public:
    
    void prepare(int sampleRate){
        buffer.prepare(sampleRate);
    };
    void update(BurstState currentState){
        if(currentState == BurstState::good){currentMode = Mode::normal;};
        if(currentState == BurstState::bad){currentMode = Mode::conceal;};
        if(currentState == BurstState::fail){currentMode = Mode::fail;};
        
    };
    float process(float newSample){
        
        
        switch(currentMode){
            case Mode::normal:
            {
                if (previousMode == Mode::conceal || previousMode == Mode::fail){
                    float fadeGain = buffer.fadeInGain();
                    if (buffer.fadePos == buffer.fadeLength + 1){
                        previousMode = Mode::normal;
                        buffer.fadePos = 1;
                    }
                    
                    newSample *= fadeGain;
                }

                copyBuffer = true;
                buffer.pushSample(newSample);
                
                return newSample;
                
            }
                break;
                
            case Mode::conceal:
            {
                if (previousMode == Mode::normal){previousMode = Mode::conceal;}
                
                if (copyBuffer){
                    buffer.copyToLinear();
                    buffer.startLoop();
                    temporaryBuffer = buffer.pitchBuffer;
                    copyBuffer = false;
                }
                
                return buffer.popSample();
        
            }
                break;
                
            case Mode::fail:
            {
                previousMode = Mode::fail;
                return 0;
            }
                break;
                
            default:
                return 0;
        }
        

        
        return 0;
    }
    void reset(){
        buffer.reset();
    }
    
    // This is coming along nicely but currently the process function will copy the buffer everytime the bad state is read. So I need some way to eternally loop through the pitch buffer until the state returns to good. I have also purposely left loss concealment as two instances for L/R because I want to have different gilbert Elliot states for L/R audio in the future. We'll cross that bridge when we get to it though..
    

    
    private:
    
    enum class Mode {normal, conceal, fail};
    
    Mode currentMode = Mode::normal;
    Mode previousMode = Mode::normal;
    
    bool copyBuffer = true;
    
    
    LossCircularBuffer buffer;
    LossCircularBuffer::Buffer temporaryBuffer;
    

};

/*
 This class is going to be a weird one to work on because the stereoDecoder.getNextSample is going to feed the loss circular buffer. Then when this class is given the bad state the concealment will begin. So the gilbert elliot logic needs to exist here too.. At this point I need to create a class which means that the next sample output is coming from the temporary non circular buffer (pitch buffer).
 
 All good samples fed into circular buffer (safety buffer)
 
 As soon as state turns bad the entire safety buffer is copied into a linear array.
 
 Then this pitch buffer is looped until the state becomes good again.
 
 When the state becomes good again the samples are read from the stereoDecoder once again.
 */
