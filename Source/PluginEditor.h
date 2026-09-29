/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "RotaryKnob.h"
#include "Parameters.h"
#include "LookAndFeel.h"
#include "Colours.h"

//==============================================================================
/**
*/

enum class PedalState { Bypassed, Pressed, Active };

class BadBluetoothProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BadBluetoothProcessorEditor (BadBluetoothProcessor&);
    ~BadBluetoothProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BadBluetoothProcessor& audioProcessor;

    RotaryKnob bitPoolKnob{"BitPool", audioProcessor.apvts, bitPoolParamID, false};
    RotaryKnob sensitivityKnob{"Sensitivity", audioProcessor.apvts, bitPoolResolutionParamID, false};
    RotaryKnob distanceKnob{"Distance", audioProcessor.apvts, distanceParamID, false};
    RotaryKnob materialKnob{"Material", audioProcessor.apvts, materialParamID, false};
    RotaryKnob concealmentKnob{"Concealment", audioProcessor.apvts, concealmentTypeParamID, false};

    juce::Label bitPoolLabel;
    juce::Label bitPoolResolutionLabel;

    juce::Image bypassedImage, pressedImage, activeImage;
    PedalState currentState = PedalState::Bypassed;
    
    float pi = juce::MathConstants<float>::pi;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BadBluetoothProcessorEditor)
};
