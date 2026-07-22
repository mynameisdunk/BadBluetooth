
#pragma once
#include <JuceHeader.h>

class ScaleFactoring{
    
    public:
    
    void prepare();
    void update();
    
    std::array<float, 8> process(std::array<float, 8> block){
        
        for (int i = 0; i < 8; i++){
            if (block[i] > scaleFactors[i])
            {
                scaleFactors[i] = block[i];
            }
        }
        
        return scaleFactors;
    }
    
    
    std::array<float, 8> getScalefactor(){
        return scaleFactors;
    }
    
    
    private:
    
    std::array<float, 8> scaleFactors{};
    
    std::array<float, 8> quantise(std::array<float, 8> scaleFactors){
        
        for (int i = 0; i < 8; i++){
            this->scaleFactors[i] = std::pow (2.0f, scaleFactors[i] + 1.0f);
        }
        
        return scaleFactors;
    }
    
};

