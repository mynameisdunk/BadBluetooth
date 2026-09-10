/*
  ==============================================================================

    FilmStripLookAndFeel.h
    Created: 24 Aug 2026 7:21:56pm
    Author:  Harry Dunk

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
class FilmStripLookAndFeel : public juce::LookAndFeel_V4
{
public:
    
    FilmStripLookAndFeel() = default;
    
    void setStripImage(const juce::Image& image, int numFrames, bool isHorizontalStrip = false)
    {
        stripImage = image;
        frameCount = numFrames;
        isHorizontal = isHorizontalStrip;
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float, float, juce::Slider&) override
    {
        if (stripImage.isNull() || frameCount <= 0)
            return;

        int frameIndex = juce::jlimit(0, frameCount - 1,
                                       (int) std::round(sliderPos * (float) (frameCount - 1)));

        int frameW = isHorizontal ? stripImage.getWidth() / frameCount : stripImage.getWidth();
        int frameH = isHorizontal ? stripImage.getHeight() : stripImage.getHeight() / frameCount;

        g.drawImage(stripImage, x, y, width, height,
                     isHorizontal ? frameIndex * frameW : 0,
                     isHorizontal ? 0 : frameIndex * frameH,
                     frameW, frameH);
    }
    
    bool isOpaqueAt (int x, int y, double normalisedSliderPos) const
        {
            if (stripImage.isNull() || frameCount <= 0)
                return false;

            int frameIndex = juce::jlimit(0, frameCount - 1,
                                           (int) std::round(normalisedSliderPos * (double) (frameCount - 1)));

            int frameW = isHorizontal ? stripImage.getWidth() / frameCount : stripImage.getWidth();
            int frameH = isHorizontal ? stripImage.getHeight() : stripImage.getHeight() / frameCount;

            int srcX = isHorizontal ? frameIndex * frameW + x : x;
            int srcY = isHorizontal ? y : frameIndex * frameH + y;

            if (srcX < 0 || srcY < 0 || srcX >= stripImage.getWidth() || srcY >= stripImage.getHeight())
                return false;

            return stripImage.getPixelAt(srcX, srcY).getAlpha() > 20;
        }

private:
    juce::Image stripImage;
    int frameCount = 0;
    bool isHorizontal = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilmStripLookAndFeel)
};
