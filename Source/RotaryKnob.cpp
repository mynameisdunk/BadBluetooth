#include <JuceHeader.h>
#include "RotaryKnob.h"
#include "PluginEditor.h"
#include "Parameters.h"
#include "PluginProcessor.h"
#include "LookAndFeel.h"

//==============================================================================
RotaryKnob::RotaryKnob(const juce::String& text,
                       juce::AudioProcessorValueTreeState& apvts,
                       const juce::ParameterID& parameterID,
                       bool drawFromMiddle,
                       bool showTextBox)
: attachment(apvts, parameterID.getParamID(), slider)

{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
//    slider.setTextBoxStyle(showTextBox ? juce::Slider::TextBoxBelow : juce::Slider::NoTextBox, false, 70, 16);
    
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    //                                            textbox has no depth - does not affect slider bounds
    slider.setBounds(0, 0, 128, 128);
    addAndMakeVisible(slider);
    
/*
    label.setText(text, juce::NotificationType::dontSendNotification);
    label.setJustificationType(juce::Justification::horizontallyCentred);
    label.setBorderSize(juce::BorderSize<int>{0, 0, 0, 0});
    label.attachToComponent(&slider, false);
    addAndMakeVisible(label);
*/
    setLookAndFeel(RotaryKnobLookAndFeel::get());
    
    float pi = juce::MathConstants<float>::pi;
    slider.setRotaryParameters(1.3f * pi, 2.7f * pi, true);
    
    slider.getProperties().set("drawFromMiddle", drawFromMiddle);
    
    setSize( 128,128);
}

void RotaryKnob::setFilmStrip(const juce::Image& stripImage, int numFrames, bool isHorizontal)
{
    filmStripLookAndFeel = std::make_unique<FilmStripLookAndFeel>();
    filmStripLookAndFeel->setStripImage(stripImage, numFrames, isHorizontal);
    slider.setLookAndFeel(filmStripLookAndFeel.get());
}

RotaryKnob::~RotaryKnob()
{
    slider.setLookAndFeel(nullptr);
    
}

 

void RotaryKnob::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    slider.setTopLeftPosition(0, 0);
    
    auto r = getLocalBounds();

       // Reserve bottom strip for the label (or for visual padding if using slider's TextBoxBelow)
//       const int labelHeight = 16;
//       auto labelArea = r.removeFromBottom(labelHeight);

       // Create a square area for the knob (slider) centered in the remaining space
       int side = juce::jmin(r.getWidth(), r.getHeight());
       juce::Rectangle<int> knobSquare = r.withSizeKeepingCentre(side, side);

       // Place slider inside the square
       slider.setBounds(knobSquare);

       // If you're using your own label (not attachToComponent), place it here
//       label.setBounds(0, slider.getBottom(), getWidth(), labelHeight);
}


bool RotaryKnob::hitTest (int x, int y)
{
    // If no film strip is active, fall back to the default rectangular hit-test
    if (filmStripLookAndFeel == nullptr)
        return juce::Component::hitTest(x, y);

    double normalisedPos = slider.valueToProportionOfLength(slider.getValue());
    return filmStripLookAndFeel->isOpaqueAt(x, y, normalisedPos);
}
