#pragma once
#include <JuceHeader.h>

const juce::ParameterID gainParamID {"gain", 1};
const juce::ParameterID bitPoolParamID {"bitPool", 1};
const juce::ParameterID bitPoolResolutionParamID {"bitPoolResolution", 1};

class Parameters
{
public:
    Parameters(juce::AudioProcessorValueTreeState& apvts);
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    void update() noexcept;
    void prepareToPlay(double sampleRate) noexcept;
    void reset() noexcept;
    void smoothen() noexcept;
    
    
    
    
    float gain = 0.0f;
    float bitPoolResolution = 0.0f;
    
    int bitPool = 16;
    
    
private:
    
    juce::AudioParameterFloat* gainParam;
    juce::LinearSmoothedValue<float> gainSmoother;
    
    juce::AudioParameterInt* bitPoolParam;
    
    juce::AudioParameterFloat* bitPoolResolutionParam;
    juce::LinearSmoothedValue<float> bitPoolResolutionSmoother;
    
};
