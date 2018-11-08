#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include "Sim.h"

class Memory
{
private:

    //member data 
    uint64_t mem[MEMORY_SIZE];
    bool memError;
  
    //member functions
    void store(uint64_t waddr, uint64_t val);
    uint64_t fetch(uint64_t waddr);
    
public:

    //member functions
    Memory();
    unsigned char getByte(uint64_t byteAddress);
    void putByte(uint64_t byteAddress, uint8_t value);
    uint64_t getWord(uint64_t byteAddress);
    void putWord(uint64_t byteAddress, uint64_t wordValue);
    void reset(void)
    {
        for (int i = 0; i < MEMORY_SIZE; i++)
        {
            mem[i] = 0;
			
        }
        memError = false;
    }
    bool isError(void)
    {
        return memError;
    }
};

#endif
