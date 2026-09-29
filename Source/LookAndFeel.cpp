#include "LookAndFeel.h"


juce::Font MainLookAndFeel::getLabelFont([[maybe_unused]] juce::Label& label)
{
    return Fonts::getFont();
}

juce::Font MainLookAndFeel::getTextButtonFont(juce::TextButton& button, int buttonHeight)
{
    return Fonts::getFont(juce::jlimit(12.0f, 20.0f, (float) buttonHeight * 0.5f));
}

juce::Font MainLookAndFeel::getPopupMenuFont()
{
    return Fonts::getFont(14.0f);
}

juce::Font MainLookAndFeel::getComboBoxFont(juce::ComboBox&)
{
    return Fonts::getFont(14.0f);
}

juce::Font MainLookAndFeel::getAlertWindowTitleFont()
{
    return Fonts::getFont(18.0f);
}

juce::Font MainLookAndFeel::getAlertWindowMessageFont()
{
    return Fonts::getFont(14.0f);
}
