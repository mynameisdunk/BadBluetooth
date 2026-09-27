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
    params.reset();
    params.prepareToPlay(sampleRate);
    
    juce::dsp::ProcessSpec monoSpec;
    monoSpec.sampleRate = sampleRate;
    monoSpec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    monoSpec.numChannels = 1;
    
    juce::dsp::ProcessSpec stereoSpec;
    stereoSpec.sampleRate = sampleRate;
    stereoSpec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    stereoSpec.numChannels = 2;
    
   
    
    basicFiltersL.reset();
    basicFiltersL.prepare(monoSpec);
    basicFiltersR.reset();
    basicFiltersR.prepare(monoSpec);
    
    stereoDecoder.reset();
    stereoDecoder.prepare(stereoSpec);
  
// set sbcParameter values (duh)
    sbcParameters.sampleRate = sampleRate;
    sbcParameters.bitPool = params.bitPool;
    sbcParameters.concealmentType = params.concealmentType;
    
//    frameAssemblyL.prepare(sbcParameters);
//    frameAssemblyR.prepare(sbcParameters);
    
    pathLoss.prepare(params.distance, params.material);
    
    stateMachine.reset();
    stateMachine.prepare(static_cast<int>(sampleRate));
    
    
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
#endif //

void BadBluetoothProcessor::processBlock (juce::AudioBuffer<float>& buffer, [[maybe_unused]] juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    
    params.update();
    stateMachine.update(params.concealmentType);
    
    sbcParameters.bitPool = params.bitPool;
    sbcParameters.bitPoolResolutionScaling = params.bitPoolResolution;

    frameAssembly.update(sbcParameters);
    
    pathLoss.update(params.distance, params.material);
    float L = pathLoss.calculatePathLoss();
//    DBG(L);
    
    float* channelDataL = buffer.getWritePointer(0);
    float* channelDataR = buffer.getWritePointer(1);
    
    for (int samp = 0; samp < buffer.getNumSamples(); ++samp )
        {
            params.smoothen();
            
          float dryL = basicFiltersL.inputProcess(0, channelDataL[samp]);
          float dryR = basicFiltersR.inputProcess(0, channelDataR[samp]);
            
            auto stereoBlock = encodeBuffer.process({dryL, dryR}); 
            
            float outL = 0.0f; float outR = 0.0f;
            
            // state Machine is dependent on the state of audio received
            auto output = stateMachine.process(stereoBlock, sbcParameters, L);
            
            outL = output[0];
            outR = output[1];
            
            channelDataL[samp] = basicFiltersL.outputProcess(0, outL);
            channelDataR[samp] = basicFiltersR.outputProcess(0, outR);
            
            //  I also want to add in a scrambling feature as part of one of the concealment methods which means I need to think about how that will work in both frame and sample scale.
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

