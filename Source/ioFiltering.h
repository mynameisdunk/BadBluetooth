
#pragma once
#include <JuceHeader.h>

class ioFiltering{
    
    public:
    
    ioFiltering(){
        inputHPFFreq = 20.0f;
        inputLPFFreq = 20000.0f;
        outputHPFFreq = 20.0f;
        outputLPFFreq = 20000.0f;
    }
    
    
    void prepare(juce::dsp::ProcessSpec& spec){
        
        inputHPF.prepare(spec);
        inputHPF.reset();
        inputHPF.setType(juce::dsp::StateVariableTPTFilterType::highpass);
        inputHPF.setCutoffFrequency(inputHPFFreq);
        
        inputLPF.prepare(spec);
        inputLPF.reset();
        inputLPF.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        inputLPF.setCutoffFrequency(inputLPFFreq);
        
        outputHPF.prepare(spec);
        outputHPF.reset();
        outputHPF.setType(juce::dsp::StateVariableTPTFilterType::highpass);
        outputHPF.setCutoffFrequency(outputHPFFreq);
        
        outputLPF.prepare(spec);
        outputLPF.reset();
        outputLPF.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        outputLPF.setCutoffFrequency(outputLPFFreq);
        
    }

    float inputProcess(int channel, float input){
        
        float output =
        inputHPF.processSample(channel, input);
        
        output =
        inputLPF.processSample(channel, output);
        
        return output;
    }
    
    float outputProcess(int channel, float input){
        
        float output =
        outputHPF.processSample(channel, input);
        
        output =
        outputLPF.processSample(channel, output);
        
        return output;
    }
    
    void reset(){
        inputHPF.reset();
        inputLPF.reset();
        outputHPF.reset();
        outputLPF.reset();
    }
    
    private:
    
    juce::dsp::StateVariableTPTFilter<float> inputHPF;
    float inputHPFFreq;
    
    juce::dsp::StateVariableTPTFilter<float> inputLPF;
    float inputLPFFreq;
    
    juce::dsp::StateVariableTPTFilter<float> outputHPF;
    float outputHPFFreq;
    
    juce::dsp::StateVariableTPTFilter<float> outputLPF;
    float outputLPFFreq;
    
};
