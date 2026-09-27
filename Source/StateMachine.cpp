#include "StateMachine.h"



void StateMachine::prepare(int sampleRate){
    concealmentType = ConcealmentType::normal;
    lossConcealmentL.prepare(sampleRate);
    lossConcealmentR.prepare(sampleRate);
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
                    
                    gilbertElliot.setState(L);
                    currentState = gilbertElliot.getState();
                    if(currentState == BurstState::good){badCounter = 0;}
                    if(currentState == BurstState::bad){badCounter++;}
                    if(badCounter >= gilbertElliot.burstDepth){currentState = BurstState::fail;}
                    
                    stereoFrame = packetLoss.process(*stereoFrame, currentState);
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
            if (stereoBlock){
                auto stereoFrame = frameAssembly.process(*stereoBlock, sbcParameters);
               
                if (stereoFrame){
                    
                    gilbertElliot.setState(L);
                    currentState = gilbertElliot.getState();
                    if(currentState == BurstState::good){badCounter = 0;}
                    if(currentState == BurstState::bad){badCounter++;}
                    if(badCounter >= gilbertElliot.burstDepth){currentState = BurstState::fail;}
                    lossConcealmentL.update(currentState);
                    lossConcealmentR.update(currentState);
                        
                    
                    stereoDecoder.process(*stereoFrame);
                    stereoFrame.reset();
                }
            }
                output[0] = lossConcealmentL.process(stereoDecoder.getNextSample(0));
                output[1] = lossConcealmentR.process(stereoDecoder.getNextSample(1));
            
            
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

void StateMachine::reset(){
    concealmentType = ConcealmentType::normal;
    badCounter = 0;
    lossConcealmentL.reset();
    lossConcealmentR.reset();
};

