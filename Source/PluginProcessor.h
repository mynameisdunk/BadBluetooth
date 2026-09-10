/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ioFiltering.h"
#include "Parameters.h"
#include "EncodeCircularBuffer.h"
#include "FrameAssembly.h"
#include "DataStructures.h"
#include "Decoder.h"
#include "PathLoss.h"
#include "PacketLoss.h"



//==============================================================================
/**
*/
class BadBluetoothProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    BadBluetoothProcessor();
    ~BadBluetoothProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState apvts {
            *this, nullptr, "Parameters", Parameters::createParameterLayout()
        };
private:
    
    
// INSTANCES

    ioFiltering basicFiltersL;
    ioFiltering basicFiltersR;
    
    Parameters params;
    
    EncodeCircularBuffer cBufferL;
    EncodeCircularBuffer cBufferR;
    
    EncodeCircularBuffer encodeBuffer;
    
    FrameAssembly frameAssemblyL;
    FrameAssembly frameAssemblyR;
    
    SBCParameters sbcParameters;
    
    Decoder decoderL;
    Decoder decoderR;
    
    PathLoss pathLoss;
    
    PacketLoss packetLossLeft;
    PacketLoss packetLossRight;
    
    
    
// PRIVATE VARIABLES
    
    std::array<float, 80> weightedL{};
    std::array<float, 80> weightedR{};
    
    std::optional<EncodedFrame> frameL;
    std::optional<EncodedFrame> frameR;
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BadBluetoothProcessor)
};
