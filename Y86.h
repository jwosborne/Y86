/*
    File:   Y86.h
    Desc:   Y86 constants 
    
*/

#ifndef Y86_H
#define Y86_H

#include <iostream>
#include <cstdint>
#include <string>
#include "Memory.h"
#include "ProgRegisters.h"
#include "Y86Loader.h"
#include "FetchStage.h"
#include "DecodeStage.h"
#include "ExecuteStage.h"
#include "MemoryStage.h"
#include "WritebackStage.h"
#include "PipeStage.h"
#include "Forward.h"


class Y86 {
	
    /* class objects */
    Memory              memory;          
    ProgRegisters       regs;
	Y86Loader			loader;
	Forward				forward;
	
	FetchStage fetchStage;
	DecodeStage decodeStage;
	ExecuteStage executeStage;
	MemoryStage memoryStage;
	WritebackStage writebackStage;

	PipeStage *stage[5];  // an array of 5 PipeStage pointers

	int cycles;           // cycle count	
	
    /* Private member functions */
	bool 		readFile		(std::ifstream& infile);
	int 		writeMemory		(std::string inst, uint64_t address);
    void 		getLine			(uint64_t *, uint64_t);
	
	
	
	std::string getFlagsString	(void); // in Y86trace.cpp
	
    /* Public member functions */
    public:
        Y86();
        void reset();
		void clock();
		bool load(char *fname);  // takes the name of an input file
		void dumpMemory();			
		void dumpProgramRegisters();
		void dumpProcessorRegisters();
		void setTrace(bool f, bool d, bool e, bool m, bool w);
		void trace(void);
		uint64_t getStat();
		int getCycles(){return cycles;}; //Note that this is implemented here
		//lab12
		void pipeControl();  
		

        Memory& getMemory(){return memory;}  
        ProgRegisters& getProgRegisters(){return regs;}
};

#endif
