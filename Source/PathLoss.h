/*
  ==============================================================================

    PathLoss.h
    Created: 31 Aug 2026 1:19:59pm
    Author:  Harry Dunk

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


/*
                        STRUCTURING THE PATH LOSS CALCULATIONS
                                (or at least trying to)
 
 L = 10n log(d) + C

 L = Path Loss in dB
 n = Path Loss Component
 d = Distance between transmitter and receiver
 C = Constant accounting for system losses
 
 
 L is the variable that is going to inform the missing data reconstrcution functions how to act. If L is <70dB then it will tell it to do nothing
 
 n is the parameter which will be decided using a list of optional materials to sit between the transmitter and receiver
 
 d is the distance between the two devices.
 
 C should be ignored in this implementation as I can assume that the system is perfect
 
 
 
 This function should sit between the encoding and decoding stages. It is describing to the decoder how clean the data it is receiving is.
 
 When the value of L > 70dB the decoder will start to be fed data based on the path loss and gilber elliot model. This is where I need to implement another buffer which is storing each packet as it comes in just in case the next one doesn't arrive in good condition.
 
 Each time a packet is delivered it should use the value of L to decide whether or not it can be decoded. It will also feed into the gilbert elliot model which is telling the buffer the probability that it should play the next packet.

 
 */

class PathLoss{
    
    public:
    
    void prepare(float distance, float material){
        d = distance;
        n = material;
    }
    
    void update(float distance, float material){
        d = distance;
        n = material;
    }
    
    float calculatePathLoss(){
        L = (10.0f * n) * std::log(d);
        
        return L;
    }
    
    
    private:
    
    float L = 0.0f;
    float n = 0.0f;
    float d = 0.0f;
    
    
};
