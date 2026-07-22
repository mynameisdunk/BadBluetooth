
#pragma once

#include <JuceHeader.h>


class BitAllocation{
    public:
    
    enum class AllocationMethod{
        LOUDNESS = 0,
        SNR = 1
    };
    
    std::array<int, 8> process(std::array<float, 8> scaleFactor){
        
/*        juce::String s;
            for (auto v : scaleFactor) s << v << " ";
            DBG ("scaleFactor going into BitAllocation: " << s);
        */
        
        allocationMethod = AllocationMethod::SNR;
                                            //^^ MAKE PARAMETER
        bitNeedCalculation(scaleFactor);
        setMaxBitneed(bitneed);
        calculateBitSlices();
        allocateBits();

        return bits;
    }
    
    private:
    
    // Variables
    std::array<float, 8> bitneed{};
    std::array<float, 8> loudness{};
    std::array<int, 8> bits{};
    
    float maxBitneed = 0.0f;
    int bitCount = 0;
    int sliceCount = 0;
    int bitSlice = 0;
    int bitPool = 52;
               // ^^ MAKE PARAMETER
    
    // Instances
    
    AllocationMethod allocationMethod;
    // Functions
    
    std::array<float, 8> bitNeedCalculation(std::array<float, 8> scaleFactor){
        
        if (allocationMethod == AllocationMethod::SNR){
            for (int i = 0; i < 8; i++){
                bitneed[i] = scaleFactor[i];
            }
        }
        
        else {
            for (int i = 0; i < 8; i++){
                if (scaleFactor[i] == 0){
                    bitneed[i] = -5;
                }
                
                else{
                    loudness[i] = scaleFactor[i] - offset844k[i];
                    
                    if (loudness[i] > 0){
                        bitneed[i] = loudness[i] / 2.0f;
                    }
                    
                    else bitneed[i] = loudness[i];
                }
            }
        }
        return bitneed;
    }
    
    void setMaxBitneed(std::array<float, 8> bitNeed){
        
        maxBitneed = 0;
        
        for (int i = 0; i < 8; i++){
            if (bitNeed[i] > maxBitneed){
                maxBitneed = bitNeed[i];
            }
        }
    }
    
    void calculateBitSlices(){
        bitCount = 0;
        sliceCount = 0;
        
        bitSlice = maxBitneed + 1;
        
        int safetyCounter = 0;
        
        do {
            bitSlice --;
            bitCount += sliceCount;
            sliceCount = 0;
            for (int i = 0; i < 8; i++){
                if((bitneed[i]>bitSlice+1)&&(bitneed[i]<bitSlice+16)){
                    sliceCount ++;
                }
                else if (bitneed[i] == bitSlice + 1){
                    sliceCount += 2;
                }
            }
            
            
            safetyCounter++;
                    if (safetyCounter > 1000)
                    {
                        DBG ("calculateBitSlices STUCK - bitSlice= " << bitSlice << " bitCount= " << bitCount << " sliceCount= " << sliceCount);
                        break; // force exit no matter what, so we can at least see the DBG output
                    }
            
        }
        while (bitCount + sliceCount < bitPool && (bitSlice > -16));
        
        if (bitCount + sliceCount == bitPool){
            bitCount += sliceCount;
            bitSlice --;
        }
    }
        
    void allocateBits(){
        for (int i = 0; i < 8; i++){
            if (bitneed[i] < bitSlice + 2){
                bits[i] = 0;
            }
            
            else {
                bits[i] = fmin(bitneed[i] - bitSlice, 16);
            }
        }
    }
        
        
        // Data
        // offset 8
        std::array<int, 8> offset816k = {-2, 0, 0, 0, 0, 0, 0, 1};
        std::array<int, 8> offset832k = {-3, 0, 0, 0, 0, 0, 1, 2};
        std::array<int, 8> offset844k = {-4, 0, 0, 0, 0, 0, 1, 2};
        std::array<int, 8> offset848k = {-4, 0, 0, 0, 0, 0, 1, 2};
        
        std::array<std::array<int, 8>, 4> offset8 = {offset816k, offset832k, offset844k, offset848k};
        
        // offset 4
        std::array<int, 4> offset416k = {-1, 0, 0, 0};
        std::array<int, 4> offset432k = {-2, 0, 0, 1};
        std::array<int, 4> offset444k = {-2, 0, 0, 1};
        std::array<int, 4> offset448k = {-2, 0, 0, 1};
        
        std::array<std::array<int, 4>, 4> offset4 = {offset416k, offset432k, offset444k, offset448k};
        
        
        
        
};

