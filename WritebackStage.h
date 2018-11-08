/*
    File:   WritebackStage.h
    Desc:   Declares WritebackStage class and associated constants
    
*/
#ifndef WRITEBACKSTAGE_H
#define WRITEBACKSTAGE_H


#include "Sim.h"
#include "PipeStage.h"
#include "Register.h"
#include "ProgRegisters.h"
#include "Forward.h"


class WritebackStage : public PipeStage
{
	
    /* Pointers to Execute Stage and Program Registers object */
	ProgRegisters *regs;
	Forward		*forward;
	
	
    /* signals produced within the stage */
    Register<uint64_t> valE;
    Register<uint64_t> valM;
    Register<uint64_t> dstE;
    Register<uint64_t> dstM;
	
	uint64_t w_stat; 
	uint64_t w_icode;
    uint64_t w_ifun;
	uint64_t w_valE;
	uint64_t w_valM;
	uint64_t w_dstE;
	uint64_t w_dstM;
	
	public:
		void reset(ProgRegisters *, Forward *);
		void updateWRegister(uint64_t, uint64_t, uint64_t, uint64_t, 
								uint64_t, uint64_t, uint64_t);

		/* (Virtual) Functions of superclass */
		void clockP0();
		void clockP1();
		void trace();
};

#endif
