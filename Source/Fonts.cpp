/*
  ==============================================================================

    Fonts.cpp
    Created: 28 Sep 2026 2:46:29pm
    Author:  Harry Dunk

  ==============================================================================
*/

#include "Fonts.h"

const juce::Typeface::Ptr Fonts::typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::GoogleSansCodeBold_ttf, BinaryData::GoogleSansCodeBold_ttfSize);

juce::Font Fonts::getFont(float height)
{
    return juce::FontOptions(typeface) .withMetricsKind(juce::TypefaceMetricsKind::legacy)
                        .withHeight(height);
}
