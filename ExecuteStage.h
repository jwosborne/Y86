/*
    File:   ExecuteStage.h
    Desc:   Declares ExecuteStage class and associated constants
    
*/
#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H


#include "Sim.h"
#include "PipeStage.h"
#include "Register.h"
#include "MemoryStage.h"
#include "Forward.h"
#include "ProgRegisters.h"


class ExecuteStage : public PipeStage
{
	
    /* Pointers to Execute Stage and Program Registers object */
	MemoryStage *memoryStage;
	ProgRegisters *regs;
	Forward		*forward;
	
	
    /* signals produced within the stage */
    Register<uint64_t> valC;
    Register<uint64_t> valA;
    Register<uint64_t> valB;
    Register<uint64_t> dstE;
    Register<uint64_t> dstM;
    Register<uint64_t> srcA;
    Register<uint64_t> srcB;
	
    int zf;
    int sf;
    int of;
	
	uint64_t e_stat;
	uint64_t e_icode;
	uint64_t e_ifun;
    uint64_t e_valE; 
	uint64_t e_valA;
	uint64_t e_dstE;
	uint64_t e_dstM;
	bool e_cnd;

	int64_t aluA;
	int64_t aluB;
	uint64_t aluFun;
	
	public:
		void reset(MemoryStage *, ProgRegisters *, Forward *);
		void updateERegister(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

		void getALUA();
		void getALUB();
		void getALUFunction();
		bool setsCC();
		void setCCodes(int, int, int);
		void selectDstE();
		bool isOverflow();
		bool setCnd();
		bool getCnd(){return e_cnd;}
		uint64_t getDstM(){return e_dstM;} 
		uint64_t highBit(uint64_t);
		
		/* (Virtual) Functions of superclass */
		void clockP0();
		void clockP1();
		void trace();
};

#endif
