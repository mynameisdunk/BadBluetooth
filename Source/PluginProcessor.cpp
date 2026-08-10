/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BadBluetoothProcessor::BadBluetoothProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       ), params(apvts)
{
}

BadBluetoothProcessor::~BadBluetoothProcessor()
{
}

//==============================================================================
const juce::String BadBluetoothProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BadBluetoothProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BadBluetoothProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BadBluetoothProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BadBluetoothProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BadBluetoothProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BadBluetoothProcessor::getCurrentProgram()
{
    return 0;
}

void BadBluetoothProcessor::setCurrentProgram (int index)
{
}

const juce::String BadBluetoothProcessor::getProgramName (int index)
{
    return {};
}

void BadBluetoothProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BadBluetoothProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    params.prepareToPlay(sampleRate);
    params.reset();
    
  
// set sbcParameter values (duh)
    sbcParameters.sampleRate = sampleRate;
    
//    frameAssemblyL.prepare(sbcParameters);
//    frameAssemblyR.prepare(sbcParameters);
}

void BadBluetoothProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BadBluetoothProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}
#endif

void BadBluetoothProcessor::processBlock (juce::AudioBuffer<float>& buffer, [[maybe_unused]] juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    
    params.update();
    
    float* channelDataL = buffer.getWritePointer(0);
    float* channelDataR = buffer.getWritePointer(1);
    
    for (int samp = 0; samp < buffer.getNumSamples(); ++samp )
        {
            float dryL = channelDataL[samp];
            float dryR = channelDataR[samp];
            
            auto bufferL = cBufferL.process(dryL);
            auto bufferR = cBufferR.process(dryR);
            
            
//            static int outerFireCount = 0;
            
            float outL = 0.0f;
            float outR = 0.0f;

            if (bufferL && bufferR){
                //                outerFireCount++;
                //                DBG ("outer fired, count = " << outerFireCount);
                
                frameL = frameAssemblyL.process(*bufferL, sbcParameters);
                frameR = frameAssemblyR.process(*bufferR, sbcParameters);
                
                //                if (frameL.has_value()){
                //                    DBG ("got Frame");
                //                }
                //                else DBG ("BUG");
                if(frameL && frameR){
                                
                                decoderL.process(*frameL);
                                decoderR.process(*frameR);
                            }
            }
                
            
                    
            outL = decoderL.getNextSample();
            outR = decoderR.getNextSample();
            
            channelDataL[samp] = outL;
            channelDataR[samp] = outR;
        }
}

//==============================================================================
bool BadBluetoothProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BadBluetoothProcessor::createEditor()
{
    return new BadBluetoothProcessorEditor (*this);
}

//==============================================================================
void BadBluetoothProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    copyXmlToBinary(*apvts.copyState().createXml(), destData);
}

void BadBluetoothProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName(apvts.state.getType())) {
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BadBluetoothProcessor();
}

