#include "StateMachine.h"



void StateMachine::prepare(){
    
}

void StateMachine::update(int paramValue){
    
    if (paramValue == 1) {concealmentType = ConcealmentType::normal;};
    if (paramValue == 2) {concealmentType = ConcealmentType::vocal;};
    


}

std::array<float, 2> StateMachine::process(StereoBlock& stereoBlock, const SBCParameters& sbcParameters, float L){
    
    switch (concealmentType){
        case ConcealmentType::normal:
        {
            if (stereoBlock){
                auto stereoFrame = frameAssembly.process(*stereoBlock, sbcParameters);
                
                if (stereoFrame){
                    stereoFrame = packetLoss.process(*stereoFrame, L);
                    stereoDecoder.process(*stereoFrame);
                    stereoFrame.reset();
                }
            }
            
            output[0] = stereoDecoder.getNextSample(0);
            output[1] = stereoDecoder.getNextSample(1);
            return output;
        }
            break;
            
            
            
        case ConcealmentType::vocal:
        {
            return output;
        }
            break;
            
        default:
        {
            if (stereoBlock){
                auto stereoFrame = frameAssembly.process(*stereoBlock, sbcParameters);
                
                if (stereoFrame){
                    stereoDecoder.process(*stereoFrame);
                    stereoFrame.reset();
                }
            }
            
            output[0] = stereoDecoder.getNextSample(0);
            output[1] = stereoDecoder.getNextSample(1);
            return output;
        }
            break;
            
    }
}

void reset(){};

