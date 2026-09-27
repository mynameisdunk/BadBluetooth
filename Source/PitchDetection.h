
#pragma once
#include <JuceHeader.h>

class PitchDetection{
    
    public:
    
    void prepare(int sampleRate){
        calculateDelay(sampleRate);
    }
    
    void process(std::array<float, 500>& originalBuffer){

        std::array<float, 900> scores;
        
        for (int d = 0; d < range; d++){
            int delay = delayInSamples[d];
            float sum = 0.0f;
            int termCount = originalBuffer.size() - delay;
            
            for (int s = 0; s < originalBuffer.size() - delay; s++){
                sum += originalBuffer[s] * originalBuffer[s + delay];
            }
            scores[d] = sum / termCount;
        }
        
        float bestScore = scores[0];
        largestIndex = 0;
        
        for (int v = 1; v < range; v++){
            if (bestScore < scores[v]){
                bestScore = scores[v];
                largestIndex = v;
            }
        }
        pitchPeriod = delayInSamples[largestIndex];
       
//        DBG("d=0: " << scores[0] << " d=150: " << scores[150] << " d=300: " << scores[300] << " d=450: " << scores[450] << " d=599: " << scores[599] << " winner=" << largestIndex);
        
    }
    
    int getPitchPeriod(){
        return pitchPeriod;
    }
    
    
    private:
    
    // -------------------------------------------------------------------------------------
    int lowestFrequency = 100;
    int highestFrequency = 400;
    int range = highestFrequency - lowestFrequency;
    
    std::array<int, 900> delayInSamples;
    
    void calculateDelay(int sampleRate){
        for (int i = 0; i < range; i++){
            int f = lowestFrequency + i;
            delayInSamples[i] = sampleRate / f;
        }
    }
    // -------------------------------------------------------------------------------------
    
    
    int largestIndex = 1;
    int pitchPeriod = 1;
  

    
    
};

/*
 This class needs to do the following:
 
 Take the original buffer
 Calculate delay times which will allign with frequencies 100 - 1000 based on the current sample rate
 
 Delay a copy of the original buffer
 Multiply each buffer value against its new corrosponding index value
 Sum the total of every multiplied value
 Store the total value against the delay length
 Compare all delay length scores to find the highest
 The winning delay length is then used as the looping period for the buffer

 
 
 */
