#pragma once
#include <JuceHeader.h>
#include "DataStructures.h"
#include "PacketLoss.h"
#include "Decoder.h"
#include "FrameAssembly.h"
#include "GilbertElliot.h"
#include "LossConcealment.h"

class StateMachine{
  
    public:
    
    void prepare(int sampleRate);
    
    void update(int paramValue);
    
    std::array<float, 2> process(StereoBlock& stereoBlock, const SBCParameters& sbcParameters, float L);
    
    void reset();
    
    
    std::array<float, 2> output{};
    std::array<float, 2> silence{};

    private:
    
    enum class ConcealmentType {normal, vocal};
    
    int badCounter = 0;
    int badCounterL = 0;
    int badCounterR = 0;
    bool splitPathLoss = false;
    
    juce::Random random;
    
    ConcealmentType concealmentType = ConcealmentType::normal;
    
    PacketLoss packetLoss;

    Decoder stereoDecoder;
    
    FrameAssembly frameAssembly;
    
    GilbertElliot gilbertElliot;
    BurstState currentState = BurstState::good;
    
    GilbertElliot gilbertElliotL, gilbertElliotR;
    BurstState currentStateL = BurstState::good, currentStateR = BurstState::good;
    
    LossConcealment lossConcealment, lossConcealmentL, lossConcealmentR;

};
