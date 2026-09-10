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
    castParameter(apvts, distanceParamID, distanceParam);
    castParameter(apvts, materialParamID, materialParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(gainParamID, "Output Gain", juce::NormalisableRange<float> {-12.0f, 12.0f}, 0));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(bitPoolParamID, "BitPoolValue", juce::NormalisableRange<float>{2.0f, 42.0f, 0.1f, 0.7f}, 16.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(bitPoolResolutionParamID, "Sensitivity", juce::NormalisableRange<float> {0.0f, 1.0f, 0.01f}, 1.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(distanceParamID, "Distance", juce::NormalisableRange<float>{0.0f, 20.0f, 0.01f}, 1.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(materialParamID, "Material", juce::NormalisableRange<float>{0.0f, 10.0f, 0.5f}, 1.0f));
    
    return layout;
}

void Parameters::update() noexcept
{
    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    
    bitPoolSmoother.setTargetValue(bitPoolParam->get());
    
    bitPoolResolutionSmoother.setTargetValue(bitPoolResolutionParam->get());
    
    distanceSmoother.setTargetValue(distanceParam->get());
    
    materialSmoother.setTargetValue(materialParam->get());
}

void Parameters::prepareToPlay(double sampleRate) noexcept
{
    double duration = 0.02;
    gainSmoother.reset(sampleRate, duration);
    bitPoolSmoother.reset(sampleRate, duration);
    bitPoolResolutionSmoother.reset(sampleRate, duration);
    distanceSmoother.reset(sampleRate, duration);
    materialSmoother.reset(sampleRate, duration);
    
}

void Parameters::reset() noexcept
{
    gain = 0.0f;
    gainSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    
    bitPool = static_cast<int>(std::round(bitPoolParam->get()))
    ;
    bitPoolSmoother.setCurrentAndTargetValue(bitPoolParam->get());
    
    bitPoolResolution = bitPoolResolutionParam->get();
    bitPoolResolutionSmoother.setCurrentAndTargetValue(bitPoolResolutionParam->get());
    
    distance = distanceParam->get();
    distanceSmoother.setCurrentAndTargetValue(distanceParam->get());
    
    material = materialParam->get();
    materialSmoother.setCurrentAndTargetValue(materialParam->get());
}

void Parameters::smoothen() noexcept
{
    gain = gainSmoother.getNextValue();
    bitPool = juce::jlimit(2, 128, static_cast<int>(std::round(bitPoolSmoother.getNextValue())));
    bitPoolResolution = bitPoolResolutionSmoother.getNextValue();
    distance = distanceSmoother.getNextValue();
    material = materialSmoother.getNextValue();
}
