#include <JuceHeader.h>
#include "RotaryKnob.h"
#include "PluginEditor.h"
#include "Parameters.h"
#include "PluginProcessor.h"
#include "LookAndFeel.h"
#include "RotaryKnobLookAndFeel.h"

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
    

    label.setText(text, juce::NotificationType::dontSendNotification);
    label.setJustificationType(juce::Justification::horizontallyCentred);
    label.setBorderSize(juce::BorderSize<int>{0, 0, 0, 0});
    label.attachToComponent(&slider, false);
    addAndMakeVisible(label);

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

    auto r = getLocalBounds();

       // Create a square area for the knob (slider), pinned to the top of the bounds
       int side = juce::jmin(r.getWidth(), r.getHeight());
       juce::Rectangle<int> knobSquare = r.removeFromTop(side);

       // Place slider inside the square
       slider.setBounds(knobSquare);

       // Whatever's left of r (the bottom strip) is exactly the label's area
       label.setBounds(r);
}


bool RotaryKnob::hitTest (int x, int y)
{
    // If no film strip is active, fall back to the default rectangular hit-test
    if (filmStripLookAndFeel == nullptr)
        return juce::Component::hitTest(x, y);

    double normalisedPos = slider.valueToProportionOfLength(slider.getValue());
    return filmStripLookAndFeel->isOpaqueAt(x, y, normalisedPos);
}
