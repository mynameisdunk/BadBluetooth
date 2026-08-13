#pragma once

#include <JuceHeader.h>
#include "Windowing.h"




class AnalysisFilter{
    public:
    
    void prepare();
    
    void udpate();
    
    std::array<float, 8> process(std::array<float, 80> samples){
        
//        DBG(samples[50]);
        
        auto weighted = windower.process(samples);
        
//        DBG(weighted[50]);
        
        partialCalculation(weighted);
        matrixing();
        
//        DBG(subBands[0]);
        
        return subBands;
    }
    

    void reset();
    
    
    private:
    
    std::array<float, 16> partials{};
    std::array<float, 8> subBands{};
    
    void partialCalculation(std::array<float, 80> weighted){
        
        for (int i = 0; i < 16; i++){
            partials[i] = 0.0f;
            
            for (int k = 0; k < 5; k++){
                partials[i] += weighted[i + k * 16];
            }
        }
    }
    
    void matrixing(){
        for (int i = 0; i < 8; i++){
            subBands[i] = 0.0f;
            
            for (int k = 0; k < 16; k++){
            
                subBands[i] += cos((i + 0.5f) * (k - 4.0f) * M_PI / 8.0f) * partials[k];
            }
        }
    }
    
    Windower windower{false};

    
    std::array<float, 8> scaleFactors{};
    
    
};


/*
 Step 1 - Windowing
 Step 2 - Partial Calculation
 Step 3 - Matrixing
 
 
 */
