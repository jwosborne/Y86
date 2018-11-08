#include "Memory.h"
#include <assert.h>
#include <iostream>

using namespace std;
Memory::Memory()
{
    Memory::memError = false;
}

void Memory::store(uint64_t waddr, uint64_t val)
{
    if(waddr < 1024)
	{	
		Memory::memError = false;
        Memory::mem[waddr] = val;
	} 
    else
	{
		Memory::memError = true;
	}
}

uint64_t Memory::fetch(uint64_t waddr)
{
    if(waddr < 1024)
	{
		Memory::memError = false;
        return mem[waddr];	 
	}
    else
	{
        Memory::memError = true;
	}
    return 0; 
}


unsigned char Memory::getByte(uint64_t byteAddress)
{
    uint64_t word = fetch(byteAddress / 8);
    int byte = byteAddress % 8;
    word = word >> (byte * 8);
	uint8_t temp = 0xFF;
	uint8_t ret = word & temp; 
	unsigned char rett = ret;
    return rett; 
}
uint64_t putByteNumber(unsigned byteNum, uint8_t byteVal, uint64_t source)
{
	assert(byteNum < 8 && byteVal <= 0xFF);
	uint64_t temp = byteVal;
	uint64_t clear = 0xFF;
	clear = clear << byteNum * 8;
	clear = ~clear;
	
	if (temp == 0)
	{
		source &= clear;
	}
	else
	{
		source &= clear; 
		temp = temp << byteNum * 8;
		source |= temp; 
	}
    return source;                  
}
void Memory::putByte(uint64_t byteAddress, uint8_t value)
{
    uint64_t word = fetch(byteAddress / 8);
    int byte = byteAddress % 8;

	uint64_t byteU = putByteNumber(byte, value, word);
	
    store((byteAddress / 8), byteU);
}

uint64_t Memory::getWord(uint64_t byteAddress)
{
  if (byteAddress % 8 == 0)
  {
    return fetch(byteAddress / 8);
  }
  else
  {
	memError = true;
  }
  return 0;
}

void Memory::putWord(uint64_t byteAddress, uint64_t wordValue)
{
	if (byteAddress % 8 == 0) 
	{ 
		store(byteAddress / 8, wordValue);
	}
	else
	{ 
		memError = true;
	}		
}
