/*
  ==============================================================================

    Fonts.h
    Created: 28 Sep 2026 2:46:29pm
    Author:  Harry Dunk

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Fonts
{
public:
    
    Fonts () = delete;
    static juce::Font getFont(float height = 16.0f);
    
private:
    
    static const juce::Typeface::Ptr typeface;
    
};
