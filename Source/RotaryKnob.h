
#pragma once

#include <JuceHeader.h>
#include "FilmStripLookAndFeel.h"

class RotaryKnob  : public juce::Component
{
public:
    RotaryKnob(const juce::String& text,
               juce::AudioProcessorValueTreeState& apvts,
               const juce::ParameterID& paramID,
               bool drawFromMiddle = false,
               bool showTextBox = true);
    
    ~RotaryKnob() override;
    
    void resized() override;
    bool hitTest (int x, int y) override;
    void setFilmStrip(const juce::Image& stripImage, int numFrames, bool isHorizontal = false);
    
    juce::Slider slider;  //    -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -     // This is what it attaches to
//    juce::Label label;
    
    juce::AudioProcessorValueTreeState::SliderAttachment attachment;            // This is an attachment (duh) but ^^
    
    // Any attachment must be declared AFTER the component it attaches to

    
private:
    
    std::unique_ptr<FilmStripLookAndFeel> filmStripLookAndFeel;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotaryKnob)
};
