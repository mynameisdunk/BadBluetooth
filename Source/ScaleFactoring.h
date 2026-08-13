
#pragma once
#include <JuceHeader.h>

class ScaleFactoring{
    
    public:
    
    void prepare();
    void update();
    
    void process(std::array<float, 8> block){
        
        for (int i = 0; i < 8; i++){
            float absVal = std::abs(block[i]);
            if (absVal > rawMax[i])
            {
                rawMax[i] = absVal;
            }
        }
    }
    
    
    std::array<float, 8> getScaleFactor(){
        return scaleFactors;
    }
    
    std::array<int, 8> getScaleFactorIndex(){
        
        return scaleFactorIndex;
    }
    
    std::array<float, 8> quantise(){
        
        for (int i = 0; i < 8; i++){
            int index = 0;
            if (rawMax[i] > 0.0f)
            {
                index = static_cast<int> (std::ceil (std::log2 (rawMax[i]))) - 1;
                index = juce::jlimit (0, 15, index);
            }
            scaleFactorIndex[i] = index;
            scaleFactors[i] = std::pow (2.0f, static_cast<float> (index) + 1.0f);
        }
        
//        DBG(scaleFactors[7]);
        
        return scaleFactors;
    }
    
    void reset(){
        rawMax.fill(0.0f);
    }
    
    std::array<float, 8> calculateScaleFactors(std::array<int, 8> scaleFactorIndexes){
        
        std::array<float, 8> decodedScaleFactors;
        
        for (int sb = 0; sb < 8; sb++){
            
            decodedScaleFactors[sb] = std::pow(2.0, (scaleFactorIndexes[sb] + 1));
        }
        
        return decodedScaleFactors;
    }
    
    
    private:
    
    std::array<float, 8> rawMax{};
    std::array<float, 8> scaleFactors{};
    std::array<int, 8> scaleFactorIndex{};
    
    
};

