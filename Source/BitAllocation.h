
#pragma once

#include <JuceHeader.h>
#include "DataStructures.h"


class BitAllocation{
    public:
    
    enum class AllocationMethod{
        LOUDNESS = 0,
        SNR = 1
    };
    
    std::array<int, 8> process(std::array<int, 8> scaleFactorIndex, const SBCParameters& parameters)
    {
        
        allocationMethod = AllocationMethod::SNR;
//                                            //^^ MAKE PARAMETER (eventualy will respond to the kind of audio being passed. Currently limited by the separate L/R instances for processing audio. For now an acceptable limitation)
        
//        setBitPool(parameters.bitPool);
        bitPool = parameters.bitPool;
        bitNeedCalculation(scaleFactorIndex);
        setMaxBitneed(bitneed);
        calculateBitSlices();
        allocateBits();
        // A lot of this stuff needs to be addressed in a prepare to play function/ update funtion. There's no point in updating these values every time the function is called - it's just causing unnecessary work to be done

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
    int bitPool = 0;
    
    // Instances
    
    AllocationMethod allocationMethod;
    // Functions
    
    
    void bitNeedCalculation(std::array<int, 8> scaleFactorIndex)
    {
        
        if (allocationMethod == AllocationMethod::SNR)
        {
            for (int i = 0; i < 8; i++)
            {
                bitneed[i] = scaleFactorIndex[i];
            }
        }
        
        else {
            for (int i = 0; i < 8; i++)
            {
                if (scaleFactorIndex[i] == 0)
                {
                    bitneed[i] = -5;
                }
                
                else{
                    loudness[i] = scaleFactorIndex[i] - offset844k[i];
                    
                    if (loudness[i] > 0){
                        bitneed[i] = loudness[i] / 2.0f;
                    }
                    
                    else bitneed[i] = loudness[i];
                }
            }
        }
    }
    
    void setMaxBitneed(std::array<float, 8> bitNeed)
    {
        
        maxBitneed = 0;
        
        for (int i = 0; i < 8; i++){
            if (bitNeed[i] > maxBitneed)
            {
                maxBitneed = bitNeed[i];
            }
        }
    }
    
    void calculateBitSlices()
    {
        bitCount = 0;
        sliceCount = 0;
        
        bitSlice = maxBitneed + 1;
        
        int safetyCounter = 0;
        
        do
        {
            bitSlice --;
            bitCount += sliceCount;
            sliceCount = 0;
            for (int i = 0; i < 8; i++)
            {
                if((bitneed[i]>bitSlice+1)&&(bitneed[i]<bitSlice+16))
                {
                    sliceCount ++;
                }
                else if (bitneed[i] == bitSlice + 1)
                {
                    sliceCount += 2;
                }
            }
            
            
            safetyCounter++;
                if (safetyCounter > 1000)
                {
//                        DBG ("calculateBitSlices STUCK - bitSlice= " << bitSlice << " bitCount= " << bitCount << " sliceCount= " << sliceCount);
                break; // force exit no matter what, so we can at least see the DBG output
                }
            
        }
        while (bitCount + sliceCount < bitPool && (bitSlice > -16));
        
        if (bitCount + sliceCount == bitPool)
        {
            bitCount += sliceCount;
            bitSlice --;
        }
    }
        
    void allocateBits()
    {
        for (int sb = 0; sb < 8; sb++)
        {
            if (bitneed[sb] < bitSlice + 2)
            {
                bits[sb] = 0;
            }
            
            else
            {
                bits[sb] = fmin(bitneed[sb] - bitSlice, 16);
            }
        }
        
        int sb = 0;
            while (bitCount < bitPool && sb < 8)
            {
                if (bits[sb] >= 2 && bits[sb] < 16)
                {
                    bits[sb]++;
                    bitCount++;
                }
                else if (bitneed[sb] == bitSlice + 1 && bitPool > bitCount + 1)
                {
                    bits[sb] = 2;
                    bitCount += 2;
                }
                
                sb++;
            }
        
        sb = 0;
            while (bitCount < bitPool && sb < 8)
            {
                if (bits[sb] < 16)
                {
                    bits[sb] ++;
                    bitCount ++;
                }
                sb++;
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

