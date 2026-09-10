/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
BadBluetoothProcessorEditor::BadBluetoothProcessorEditor (BadBluetoothProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
// PARAMETERS
    
    bypassedImage = juce::ImageCache::getFromMemory (BinaryData::PEDAL_BODY_BYPASSED_png, BinaryData::PEDAL_BODY_BYPASSED_pngSize);
    
    pressedImage  = juce::ImageCache::getFromMemory (BinaryData::PEDAL_BODY_DOWN_png, BinaryData::PEDAL_BODY_DOWN_pngSize);
    
    activeImage   = juce::ImageCache::getFromMemory (BinaryData::PEDAL_BODY_ACTIVE_png,  BinaryData::PEDAL_BODY_ACTIVE_pngSize);
    
    auto knob1Strip = juce::ImageCache::getFromMemory (BinaryData::KNOB_LEFT_STRIP_png,BinaryData::KNOB_LEFT_STRIP_pngSize);
    
    auto knob2Strip = juce::ImageCache::getFromMemory (BinaryData::KNOB_RIGHT_STRIP_png,BinaryData::KNOB_RIGHT_STRIP_pngSize);
    
    
    bitPoolKnob.setFilmStrip (knob1Strip, 128);
    bitPoolResolutionKnob.setFilmStrip (knob2Strip, 128);
    addAndMakeVisible(bitPoolKnob);
    addAndMakeVisible(bitPoolResolutionKnob);

    
    setSize (480, 480);
}

BadBluetoothProcessorEditor::~BadBluetoothProcessorEditor()
{
}

//==============================================================================
void BadBluetoothProcessorEditor::paint (juce::Graphics& g)
{
    /*

// (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::black);

// BACKGROUND COLOUR
    g.setColour(juce::Colours::aliceblue);
    g.fillRoundedRectangle(2, 2, 296, 296, 6);
    
// HEADER COLOUR
    g.setColour(juce::Colours::darkgrey);
    g.fillRoundedRectangle(2, 2, 296, 45, 6);
    
    g.setFont(Fonts::getFont(32.0f));
    g.setColour(juce::Colours::black);
    g.drawText("BAD BLUETOOTH", 4, 4, 296, 40, juce::Justification::centred);
    
*/
    
// DBG BOUNDS
//    g.setColour(juce::Colours::red);
//    g.drawRect(bitPoolKnob.getBounds(), 2);
    
    juce::Image* current = &bypassedImage;
    if (currentState == PedalState::Pressed) current = &pressedImage;
    else if (currentState == PedalState::Active) current = &activeImage;

    g.drawImage (*current, getLocalBounds().toFloat());
    
}

void BadBluetoothProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
//    bitPoolKnob.setBounds(38, 55, 92, 92);
//    bitPoolLabel.setBounds(bitPoolKnob.getX(), bitPoolKnob.getBottom() + 4, 92, 16);
//    
//    bitPoolResolutionKnob.setBounds(166, 55, 92, 92);
//    bitPoolResolutionLabel.setBounds(bitPoolResolutionKnob.getX(), bitPoolResolutionKnob.getBottom() + 4, 92, 16);
    
    bitPoolResolutionKnob.setBounds(0, 0, 480, 480);
    bitPoolKnob.setBounds(0, 0, 480, 480);
    
    
    
}
