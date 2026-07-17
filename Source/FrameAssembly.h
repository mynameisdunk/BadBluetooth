
#pragma once

#include <JuceHeader.h>
#include "AnalysisFilter.h"

class FrameAssembly{
    public:
    
    using Frame = std::array<std::array<float, 8>, 16>;
    //                          16 sets of arrays that contain 8 subbands each
    

    
    void prepare();
    
    void update();
    
    //process function
    std::optional<Frame> process (std::array<float, 80> samples){
        // return type is either a Full frame or nothing
        
        frame[frameCount] = analysisFilter.process(samples);
        frameCount++;
        
        if (frameCount ==16)
        {
            frameCount = 0;
            return frame;
        }
        
        return std::nullopt;
        
    };

    void reset();
    
    private:
    
    int frameCount = 0;
    
    float block[8];
    
    Frame frame{};
    
    AnalysisFilter analysisFilter;
};
