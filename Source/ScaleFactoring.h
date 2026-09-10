
#pragma once
#include <JuceHeader.h>
#include "DataStructures.h"

class ScaleFactoring{
    
    public:
    
    void prepare();
    
    void process(std::array<float, 8> block)
    {
        
        for (int i = 0; i < 8; i++)
        {
            float absVal = std::abs(block[i]);
            if (absVal > rawMax[i])
            {
                rawMax[i] = absVal;
            }
        }
    }
    
// ------------------------------------------------------------------------------------
    // GETTER
    std::array<float, 8> getScaleFactor()
    {
        return scaleFactors;
    }
    
    // GETTER
    std::array<int, 8> getScaleFactorIndex()
    {
        return scaleFactorIndex;
    }
// ------------------------------------------------------------------------------------
    
    std::array<float, 8> quantise()
    {
        
        for (int i = 0; i < 8; i++)
        {
            int index = 0;
            if (rawMax[i] > 0.0f)
            {

                float scaledMax = rawMax[i] * baseScale;
                float sensedValue = std::log2(scaledMax) * sensitivity;
                index = static_cast<int>(std::ceil(sensedValue)) - 1;
                index = juce::jlimit (0, 15, index);
                
            }
            scaleFactorIndex[i] = index;
            scaleFactors[i] = std::pow (2.0f, static_cast<float> (index) + 1.0f) / baseScale;
        }
        
        return scaleFactors;
    }
    
// ------------------------------------------------------------------------------------
    
    void reset()
    {
        rawMax.fill(0.0f);
    }
    
// ------------------------------------------------------------------------------------
    
    std::array<float, 8> calculateScaleFactors(std::array<int, 8> scaleFactorIndexes)
    {
        
        std::array<float, 8> decodedScaleFactors;
        
        for (int sb = 0; sb < 8; sb++)
        {
            
            decodedScaleFactors[sb] = std::pow(2.0, (scaleFactorIndexes[sb] + 1)) / baseScale;
        }
        
        return decodedScaleFactors;
    }
 
// ------------------------------------------------------------------------------------
    
    void update(SBCParameters& parameters){
       
        float normalised = juce::jlimit(0.0f, 1.0f, parameters.bitPoolResolutionScaling);
        
        sensitivity = expandExponential(normalised, sensitivityMin, sensitivityMax);

    }
    
// ------------------------------------------------------------------------------------
    
    private:
    
    static float expandExponential(float normalized, float min, float max)
        {
            return min * std::pow(max / min, normalized);
        }
    
    std::array<float, 8> rawMax{};
    std::array<float, 8> scaleFactors{};
    std::array<int, 8> scaleFactorIndex{};
    
    static constexpr float baseScale = 32768.0f;
    
//    float indexScale = 1.0f;
    float reconScale = 1.0f;
    float sensitivity = 1.0f;
    
    static constexpr float sensitivityMin = 1.0f;
    static constexpr float sensitivityMax = 8.0f;
    
};

