/*
  ==============================================================================

    OutputQueue.h
    Created: 29 Jul 2026 7:53:58pm
    Author:  Harry Dunk

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <deque>

class OutputQueue {
public:
    
    // Called once per completed block (8 samples arrive together)
    void push(std::array<float, 8> newSamples){
        for (auto s : newSamples)
            queue.push_back(s);
    }
    
    // Called once per processBlock sample loop iteration (1 sample at a time)
    float pop(){
        if (queue.empty())
            return 0.0f;
        
        float sample = queue.front();
        queue.pop_front();
        return sample;
    }
    
private:
    std::deque<float> queue;
};
