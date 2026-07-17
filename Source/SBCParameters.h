/*
  ==============================================================================

    SBCParameters.h
    Created: 16 Jul 2026 3:50:12pm
    Author:  Harry Dunk

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct SBCParameters{
    
    int nrofSubbands = 8;
    int nrofBlocks = 16;
    int nrofChannels = 2;
    int samplingFrequencyIndex = 3;
    int allocationMethod = 0;
    int bitpool = 32;
};

