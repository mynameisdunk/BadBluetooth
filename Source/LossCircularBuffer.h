
#pragma once
#include <JuceHeader.h>
#include "DataStructures.h"
#include "PitchDetection.h"

class LossCircularBuffer{
    
    public:
    
    friend class LossConcealment;
    
    using Buffer = std::array<float, 500>;
    
    int bufferSize = 0;
    
    std::vector<float> adaptedBuffer;
    
    void prepare(int sampleRate){
        pitchDetection.prepare(sampleRate);
    }
    
    void pushSample(float nextSample){
        buffer[writeIndex] = nextSample;
        
        writeIndex ++;
        if (writeIndex == size){
                    writeIndex = 0;
                }
    }

    void copyToLinear() {
        for (int i = 0; i < size; i++){
            pitchBuffer[i] = buffer[(writeIndex + i) % size];
        }
        calculatePitch();
        readIndex = 0;
        adaptBuffer();
    }
    	
    
    
    void adaptBuffer(){
        
    // This class needs to be where the pitch buffer is resized (impossible with an array) so I need to create a new array of a new size
    // I believe I can do this with vectors
        pitchPeriod = pitchDetection.getPitchPeriod();
        loopLength = pitchPeriod;
        adaptedBuffer.resize(pitchPeriod);
        
        for (int i = 0; i <pitchPeriod; i++){
            adaptedBuffer[i] = pitchBuffer[i];
        }
    
    }

    //------------------------------------------------------------------------------------------
    
    struct Voice {
        bool active = false;
        int position = 0;
    };
    
    std::array<Voice, 2> voices;
    int nextSlot = 0;
    
    void startLoop() {
        voices[0] = { true, 0 };
        voices[1] = { false, 0 };
        nextSlot = 1;
    }

    float popSample() {
        float output = 0.0f;

        for (auto& v : voices) {
            if (!v.active) continue;

            float sample = adaptedBuffer[v.position] * envelope(v.position);
            output += sample;

            if (v.position == (loopLength - fadeLength)) {          // trigger #1: spawn the next head
                voices[nextSlot] = { true, 0 };
                nextSlot = 1 - nextSlot;
            }

            v.position++;
            if (v.position >= loopLength) v.active = false;          // trigger #2: retire a finished head
        }

        return output;
    }
    
    float envelope(int position) {
        if (position < fadeLength) {
            return (float)position / fadeLength;
        }
        if (position > (loopLength - fadeLength)) {
            return (float)(loopLength - position) / fadeLength;
        }
        return 1.0f;
    }
    
    
    
   // ------------------------------------------------------------------------------------------
    
    
    void reset(){
        buffer.fill(0.0f);
        readIndex = 0;
        startLoop();
    }
    
    
    private:
    
    Buffer buffer;
    Buffer pitchBuffer;
    
    int writeIndex = 0;
    int readIndex = 0;
    int size = (int)buffer.size();
    int fadeLength = 30;
    
    int fadePos = 1;
    int pitchPeriod = 1;
    int loopLength = 1;
  
    float fadeInGain(){
        
        float fadeGain = (float)fadePos / (float)fadeLength;
        fadePos++;
        
        return fadeGain;
    }
    
    void calculatePitch(){
        pitchDetection.process(pitchBuffer);
    }
    
    PitchDetection pitchDetection;
    
};
