
#pragma once

using Frame = std::array<std::array<float, 8>, 16>;
using StereoFrame = std::array<Frame, 2>;

using StereoBlock = std::optional<std::array<std::array<float, 80>, 2>>;

struct SBCParameters{
    
    int sampleRate;
    int nrofSubbands = 8;
    int nrofBlocks = 16;
    int nrofChannels = 2;
    int channelMode = 0;
    int allocationMethod = 0;
    int bitPool = 2;
    float bitPoolResolutionScaling = 1.0f;
    int concealmentType = 1;
};

struct ReconstructionValues{
    
    Frame quantisedSamples{};
    std::array<float, 8> scaleFactors{};
    std::array<int, 8> scaleFactorIndex{};
    std::array<int, 8> bitLevel{};
    
    // This can be more appropriately named HEADER INFORMATION
    
};

struct EncodedFrame{
    
    SBCParameters sbcParameters;
    ReconstructionValues encodedAudioValues;
    float concealmentGain = 1.0f;
};

using StereoEncodedFrame = std::array<EncodedFrame, 2>;


/*
                    ADDING PARAMETERS - ORDER OF PRIORITY && NOTES
 
 SampleRate
 - I don't think there is any need for this to become a parameter.
 - I only need to be sure that the plugin will work at all sample rates projects will throw at it.
 
 
 NumberOfSubbands
 - This needs to become a parameter to swap between 8 and 4 subbands of audio data.
 - This will need to be embedded in both the encoding and decoding process
 - Potentially a state machine could be used here?
 - I could create a second clas within each necessary file which runs at 4 subbands and use the state machine to decide whether the project is running on the 8 subband or 4 subband class?
 - Implementing this will be interesting as I'd want to hear that it works as the primary method before adding in the parameter to switch between the two...
 
 NumberOfBlocks
 - By default the block length for this entire project is 8
 - I think there is potential for different block lengths to create interesting creative possibilities
 - Implementing the use of multiple block lengths could be the most difficult and frustrating thing so far
 
 
 NumberOfChannels
 - This can be determined by the juce function which adresses whether the channel is in mono or stereo
 
 
 ChannelMode
 - I need to do more research into why and how each channel mode is selected
 
 
 AllocationMethod
 - This is channel mode dependent so will not be a parameter
 
 BitPool
 - DONE
 
                        REMINDER!!!
 
 All of these parameters are for the A2DP codec only
 
 IF the plugin is to be mega succesfl all of the research I've done for this allocation method will need to be repeated for the other more sophisticated methods of bluetooth audio transmission
 
 Ideally the plugin will be able to demonstrate a sound and logical representation of at least 3 kinds of bluetooth audio transmission
 
 */
