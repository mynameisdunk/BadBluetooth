#pragma once
#include <JuceHeader.h>
#include "DataStructures.h"
#include "PacketLoss.h"
#include "Decoder.h"
#include "FrameAssembly.h"

class StateMachine{
  
    public:
    
    void prepare();
    
    void update(int paramValue);
    
    std::array<float, 2> process(StereoBlock& stereoBlock, const SBCParameters& sbcParameters, float L);
    
    void reset();
    
    
    std::array<float, 2> output{};

    private:
    
    enum class ConcealmentType {normal, vocal};
    
    
    
    ConcealmentType concealmentType = ConcealmentType::normal;
    
    PacketLoss packetLoss;

    Decoder stereoDecoder;
    
    FrameAssembly frameAssembly;

};
