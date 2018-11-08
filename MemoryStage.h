/*
    File:   MemoryStage.h
    Desc:   Declares MemoryStage class and associated constants
    
*/
#ifndef MEMORYSTAGE_H
#define MEMORYSTAGE_H


#include "Sim.h"
#include "PipeStage.h"
#include "Register.h"
#include "WritebackStage.h"
#include "Memory.h"
#include "Forward.h"

class MemoryStage : public PipeStage
{
	WritebackStage 	*writeback;
	Memory 			*memory;
	ProgRegisters	*regs;
	Forward			*forward;
	
    /* signals produced within the stage */
    Register<uint64_t> valE;
    Register<uint64_t> valA;
    Register<uint64_t> dstE;
    Register<uint64_t> dstM;
    Register<bool> cnd;
	
	uint64_t m_stat;
	uint64_t m_icode;
    uint64_t m_ifun;
	uint64_t m_valA;
	uint64_t m_valE;
	uint64_t m_valM;
	uint64_t m_dstE;
	uint64_t m_dstM;
	
	bool sets;
	int zf;
	int sf;
	int of;
	
	public:
		void reset(WritebackStage *, Memory *, ProgRegisters *, Forward *);
		void updateMRegister(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, 
									uint64_t, uint64_t, bool);
		uint64_t getValue(uint64_t);
        void putValue(uint64_t, uint64_t);
		bool read();
		bool write();									
		uint64_t calcAddress();

		/* (Virtual) Functions of superclass */
		void clockP0();
		void clockP1();
		void trace();
};

#endif
