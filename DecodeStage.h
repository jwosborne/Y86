/*
    File:   DecodeStage.h
    Desc:   Declares DecodeStage class and associated constants
    
*/
#ifndef DECODESTAGE_H
#define DECODESTAGE_H


#include "Sim.h"
#include "PipeStage.h"
#include "Register.h"
#include "DecodeStage.h"
#include "ExecuteStage.h"
#include "Forward.h"


class DecodeStage : public PipeStage
{
	
    /* Pointers to Execute Stage and Program Registers object */
	ExecuteStage *executeStage;
	ProgRegisters	*regs;
	Forward		*forward;
	
	
    /* signals produced within the stage */
    Register<uint64_t> rA;
    Register<uint64_t> rB;
    Register<uint64_t> valC;
    Register<uint64_t> valP;
	
	uint64_t d_stat;
	uint64_t d_icode;
	uint64_t d_ifun;
	uint64_t d_valC;
	uint64_t d_valA;
	uint64_t d_valB;
	uint64_t d_dstE;
	uint64_t d_dstM;
	uint64_t d_srcA;
	uint64_t d_srcB;
	uint64_t d_rA;
	uint64_t d_rB;

	
	public:
		void reset(ExecuteStage *, ProgRegisters *, Forward *);
		void updateDRegister(uint64_t, uint64_t, uint64_t, uint64_t, 
								uint64_t, uint64_t, uint64_t);

		void getSrcA();
        void getSrcB();
        void getDstE();
        void getDstM();
        void selectFwdA();
        void forwardB();
		//lab12 
		uint64_t grabSrcA(){return d_srcA;}
		uint64_t grabSrcB(){return d_srcB;}
 
		
		
        /* (Virtual) Functions of superclass */
		void clockP0();
		void clockP1();
		void trace();
    
    
};

#endif
