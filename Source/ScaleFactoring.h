
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
        
        for (int i = 0; i < 8; i++){
            block[i] *= 1.0f;
                    // ^^^^^^  MUST BE REPLACED WITH QUANTISING FORMULA
        }
        
        return scaleFactors;
    }
    
    
    
    private:
    
    std::array<float, 8> scaleFactors;
    
    
    
};
