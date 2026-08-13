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
    addAndMakeVisible(bitPoolKnob);
    
// LABELS
    bitPoolLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    bitPoolLabel.setFont(Fonts::getFont(18));
    bitPoolLabel.setText("BITPOOL", juce::dontSendNotification);
    bitPoolLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(bitPoolLabel);
    
    setSize (300, 300);
}

BadBluetoothProcessorEditor::~BadBluetoothProcessorEditor()
{
}

//==============================================================================
void BadBluetoothProcessorEditor::paint (juce::Graphics& g)
{
    

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
    

    
// DBG BOUNDS
//    g.setColour(juce::Colours::red);
//    g.drawRect(bitPoolKnob.getBounds(), 2);
    
}

void BadBluetoothProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    bitPoolKnob.setBounds(10, 55, 92, 92);
    bitPoolLabel.setBounds(bitPoolKnob.getX(), bitPoolKnob.getBottom() + 4, 92, 16);
    
}
