#include "Parameters.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts,
                          const juce::ParameterID& id, T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination); // parameter does not exist or wrong type
}

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    castParameter(apvts, gainParamID, gainParam);
    castParameter(apvts, bitPoolParamID, bitPoolParam);
    castParameter(apvts, bitPoolResolutionParamID, bitPoolResolutionParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(gainParamID, "Output Gain", juce::NormalisableRange<float> {-12.0f, 12.0f}, 0));
    
    layout.add(std::make_unique<juce::AudioParameterInt>(bitPoolParamID, "BitPoolValue", 2, 128, 16));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(bitPoolResolutionParamID, "Sensitivity", juce::NormalisableRange<float> {1.0f, 1000000.0f, 1.0f}, 1.0f));
    
    return layout;
}

void Parameters::update() noexcept
{
    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    
    bitPool = bitPoolParam->get();
    
    bitPoolResolution = bitPoolResolutionParam->get();
}

void Parameters::prepareToPlay(double sampleRate) noexcept
{
    double duration = 0.02;
    gainSmoother.reset(sampleRate, duration);
    bitPoolResolutionSmoother.reset(sampleRate, duration);
}

void Parameters::reset() noexcept
{
    gain = 0.0f;
    gainSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    
    bitPool = 16;
    
    bitPoolResolution = 1.0f;
    bitPoolResolutionSmoother.setCurrentAndTargetValue(bitPoolResolutionParam->get());
}

void Parameters::smoothen() noexcept
{
    gain = gainSmoother.getNextValue();
    bitPoolResolution = bitPoolResolutionSmoother.getNextValue();
}
