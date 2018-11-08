/*
    File:   FetchStage.cpp
    Desc:   Fetch Stage implementation for Y86-64 simulator

    Authors: 
    
*/
#include "Y86.h"
#include "FetchStage.h"

/*---------------------------------------------------------------------------
    reset- used to connect to other Y86 components
     
-----------------------------------------------------------------------------*/
void FetchStage::reset(DecodeStage *pdecode, Memory *pmem, Forward *pforward)
{
		decodeStage = pdecode;  // "Connect" fetch stage to decode stage
		forward = pforward;
		memory = pmem;
		predPC.reset();
        stat.reset(SBUB);
		byteCount = {1,1,2,10,10,10,2,9,9,1,2,2,1};

}

/*---------------------------------------------------------------------------
    clockP0 - (pure virtual from PipeStage)
         Performs operations of clock Phase0 for this stage
-----------------------------------------------------------------------------*/
void FetchStage::clockP0()
{
	// Must implement--declared pure-virtual 
	// In Phase0 we:
	//     Check for STALL or BUBBLE condition, update Register variable inputs
	//     		stall/bubble come from conditions in E, M, W stages (see HCL)
	//     Call the clock method on all Register variables
	if (stageStall){
		stageStall = false;
		predPC.setInput(predPC.getState());
	}
		predPC.clock();
		f_stat = SBUB;
		f_icode = INOP;  
		f_ifun = FADDQ;
		f_rA = RNONE;
		f_rB = RNONE;
		f_valC = 0;
		f_valP = 0;
	
    
	
}
/*---------------------------------------------------------------------------
    clockP1 - (pure virtual from PipeStage)
         Performs operations of clock Phase1 for this stage
-----------------------------------------------------------------------------*/
void FetchStage::clockP1()
{
	if(!stageStall)
	{
		f_PC = selectPC();	
		
		uint8_t opCode = memory->getByte(f_PC);
		f_icode = uint64_t(opCode >> 4);
		f_ifun = uint64_t(opCode & 0xF);
		
		if(!isValidInstruction())
		{
			if(memory->isError())
				f_stat = SADR;
			else
				f_stat = SINS;  
			f_icode = 12;
			predictPC();
			predPC.setInput(f_valP);
			decodeStage->updateDRegister(f_stat, f_icode, FNONE, RNONE, RNONE, 0UL, f_valP);
		}
		else
		{
			getInstruction();
			predictPC();
			if(f_icode == IJXX || f_icode == ICALL)
				predPC.setInput(f_valC);
			else
				predPC.setInput(f_valP);
			if(f_icode != IHALT)
				f_stat = SAOK;
				
			decodeStage->updateDRegister(f_stat, f_icode, f_ifun, f_rA, f_rB, f_valC, f_valP);
			
			
		}
	}
}
/*----------------------------------------------------------------------------
   predictPC - updates predPC based on f_icode, f_valC and f_valP

------------------------------------------------------------------------------
*/
void FetchStage::predictPC()
{
	int bytes = byteCount[f_icode];
	uint64_t next = bytes + f_PC;
	f_valP = next; 
	
	if (f_icode == IJXX || f_icode == ICALL)
		f_valC = getValC(f_PC);
} 
 
bool FetchStage::isValidInstruction() 
{  
	if(f_icode >= IHALT && f_icode <= IPOPQ)
		return true;
	return false;
}

void FetchStage::add(uint64_t *inst, uint8_t byte)
{
	*inst <<= 8;
	*inst |= byte;
}

/*----------------------------------------------------------------------------
  getInstruction - (this is just an example method--something else might be better)
				returns the number of bytes in the instruction if valid,
                otherwise returns -1. Fetches remaining instruction uint64_ts
				and updates fetch stage signals:
					f_stat
					f_icode
					needsRegs
					needsValC
					f_rA
					f_rB
					f_valC
					f_valP
------------------------------------------------------------------------------
*/
int FetchStage::getInstruction()
{
	int bytes = byteCount[f_icode];		//calculating the number of bytes in our current 
	uint64_t inst = 0;					//the current instruction
	uint8_t regs = 0;					//the byte that contains rA and rB (i.e. registers)
	
	switch(bytes)						//splitting them up by the number of bytes
	{
		case 1 :
			if(f_icode == IHALT)
				f_stat = SHLT;
			
            f_rA = RNONE;				//no 1-byte instructions use rA or rB
            f_rB = RNONE;
            break;
			
		case 2 :
			regs = memory->getByte(f_PC + 1);
			
            f_rB = uint64_t(regs & 0xF);
            f_rA = uint64_t(regs >> 4);
			break;
			
		case 9 :
			for (int i = bytes - 1; i > 1; i--)
			{
				uint8_t byte = memory->getByte(f_PC + i);
				add(&inst, byte);
			}
			switch(f_icode)
			{
				case IPOPQ :
					f_rA = RSP;
					f_rB = RSP;
					break;
				case IPUSHQ :
					f_rA = RNONE;
					f_rB = RSP;
					break;
				case ICALL :
					f_rA = RNONE;
					f_rB = RNONE;
					break;
				case IRET :
					f_rA = RNONE;
					f_rB = RNONE;
					break;
				default :
					f_rA = RNONE;
					f_rB = RNONE;
					break;
			}
            f_valC = inst;				//no 9-byte instructions use rA or rB
			break;
			
		case 10 :
			regs = memory->getByte(f_PC + 1);
			for (int i = bytes - 1; i > 1; i--)
			{
				uint8_t byte = memory->getByte(f_PC + i);
				add(&inst, byte);
			}
			
			f_valC = inst;
			f_rA = uint64_t(regs >> 4);
			f_rB = uint64_t(regs & 0xF);
			break;
		default :
			break;
	}
	return bytes;
}
/*----------------------------------------------------------------------------
   getValC - returns 8-byte valC from specified uint64_t address.

------------------------------------------------------------------------------
*/
uint64_t FetchStage::getValC(uint64_t addr)
{
	return memory->getByte(addr + 1);
}
/*----------------------------------------------------------------------------
   selectPC - select next PC 

------------------------------------------------------------------------------
*/
uint64_t FetchStage::selectPC()
{
	// For now, selectPC returns predicted PC
	if(forward->getM_icode() == IJXX && !(forward->getM_Cnd()))
		return forward->getm_valA();
	else if(forward->getW_icode() == IRET)
		return forward->getW_valM();
    return predPC.getState();
}
