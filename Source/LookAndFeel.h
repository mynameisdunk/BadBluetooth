#pragma once
#include <JuceHeader.h>
#include "Colours.h"
#include "Fonts.h"
#include "RotaryKnobLookAndFeel.h"
#include "PedalButtonLookAndFeel.h"

//=======================================================================================================


class MainLookAndFeel : public juce::LookAndFeel_V4 {
public:
    MainLookAndFeel();
    
    juce::Font getLabelFont(juce::Label&) override;

    juce::Font getTextButtonFont(juce::TextButton& button, int buttonHeight) override;
    juce::Font getPopupMenuFont() override;
    juce::Font getComboBoxFont(juce::ComboBox& box) override;
    juce::Font getAlertWindowTitleFont() override;
    juce::Font getAlertWindowMessageFont() override;
    
private:
        
        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainLookAndFeel)
};
