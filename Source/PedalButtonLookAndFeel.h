/*
  ==============================================================================

    PedalButtonLookAndFeel.h
    Created: 28 Sep 2026 3:45:49pm
    Author:  Harry Dunk

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class PedalButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    
    PedalButtonLookAndFeel() = default;
    
    void drawButtonBackground (juce::Graphics& g,
                               juce::Button& button,
                               const juce::Colour&,
                               bool,
                               bool) override
    {
        
        auto bounds = button.getLocalBounds().toFloat();
        auto highlightX = bounds;
        auto highlightY = bounds;
        auto highlightTopX = bounds;
        auto highlightTopY = bounds;

        bool isOn = button.getToggleState();

        float pressOffsetX = isOn ? 1.0f : 0.0f;
        float pressOffsetY = isOn ? 1.0f : 0.0f;
        g.addTransform(juce::AffineTransform::translation(pressOffsetX, pressOffsetY));

        g.setColour (juce::Colours::black);
        g.fillRoundedRectangle (bounds, 2.0f);
// Lower Highlight
        g.setColour(juce::Colours::white.withAlpha(0.2f));
        highlightX.setBounds(13.0f, 57.0f, 106.0f, 2.0f);
        g.fillRoundedRectangle(highlightX, 0.0f);
        highlightY.setBounds(117.0f, 15.0f, 2.0f, 42.0f);
        g.fillRoundedRectangle(highlightY, 0.0f);
        
// Top Highlight
        g.setColour(juce::Colours::white.withAlpha(0.1f));
        highlightTopX.setBounds(3.0f, 3.0f, 106.0f, 2.0f);
        g.fillRoundedRectangle(highlightTopX, 0.0f);
        highlightTopY.setBounds(3.0f, 5.0f, 2.0f, 42.0f);
        g.fillRoundedRectangle(highlightTopY, 0.0f);
        
        
        
//        g.fillRoundedRectangle(12.0f,56.0f, 106.0f, 2.0f, 0.0f);
//        g.fillRoundedRectangle(116.0f, 14.0f, 2.0f, 42.0f, 0.0f);
        
        // bypassButton.setBounds(88, 440, 120, 60); // keep for reference for graphics above
        
    }
    
    private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PedalButtonLookAndFeel)
};
