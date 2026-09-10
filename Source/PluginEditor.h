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
    
    RotaryKnob bitPoolKnob{"BitPool", audioProcessor.apvts, bitPoolParamID, false, false};
    RotaryKnob bitPoolResolutionKnob{"BitPoolResolution", audioProcessor.apvts, bitPoolResolutionParamID, false, false};
    
    juce::Label bitPoolLabel;
    juce::Label bitPoolResolutionLabel;
    
    juce::Image bypassedImage, pressedImage, activeImage;
    PedalState currentState = PedalState::Bypassed;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BadBluetoothProcessorEditor)
};
