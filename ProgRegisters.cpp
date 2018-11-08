
#include <assert.h>
#include <stdio.h>
#include "ProgRegisters.h"

ProgRegisters::ProgRegisters()
{
    reset();
}

void ProgRegisters::clock()
{
    for (int i = 0; i < NUM_REGISTERS; i++){
        reg[i].clock();
    }
    CC.clock();
}

void ProgRegisters::setReg(unsigned regNum, uint64_t rval)
{ 
	assert(regNum < 16); 
	if (regNum != 15)
    {
		reg[regNum].setInput(rval);
	}
    
} 

uint64_t ProgRegisters::getReg(unsigned regNum)
{
	assert(regNum < 16 && regNum >= 0);  
    return reg[regNum].getState();
}

void ProgRegisters::setCC(unsigned bitNum, unsigned val)
{
	assert(bitNum >= 0 && bitNum < 3 && (val == 1 || val == 0));	
    uint64_t mask = 1; 
    uint64_t cc = CC.getInput();

    //CC  
    if (val == 0)
    {
  	    mask = ~(mask << bitNum);
        cc &= mask;
    }
    else
    { 
  	    mask = (mask << bitNum);
        cc |= mask;
    }
	CC.setInput(cc);
}

unsigned ProgRegisters::getCC(unsigned bitNum)
{
	uint64_t cc = CC.getState();
	return (cc >> bitNum) & 1;
}

unsigned ProgRegisters::getCC()
{
    return CC.getState();
}

void ProgRegisters::reset (void)
{
    for (int i = 0; i < NUM_REGISTERS; i++){
        reg[i].reset();
    }
    CC.reset(4);
}
