/*
    File:   main.cpp
    Desc:   The main program. Accepts and verifies parameters and executes simulator
            on the specified object file. Other parameters are used to specify the stage or
            stages where trace outputs will be printed. Trace outputs may include the 
            stage register contents, memory contents, and/or register contents. Trace outputs
            are printed at the end of a cycle.
            
*/
#include <iostream>
#include <iomanip>
#include <string>
#include "Sim.h"
#include "Y86.h"
#include "Memory.h"
#include "ProgRegisters.h"
#include <unistd.h>

#define 	MEM_SIZE_BYTES		8192

using namespace std;

Y86 y86;  // Declare global Y86 object. 

//------------------------------------------------------------------
// functions
//------------------------------------------------------------------
void usage(void);
char *getOptions(int argc, char *argv[], bool *pf, bool *pd, bool *pe, bool *pm, bool *pw);

int main(int argc, char *argv[]) 
{
    bool f = false, d = false, e = false, m = false, w = false;
    //declare and init trace flags to off
    char *nextarg = getOptions(argc, argv, &f, &d, &e, &m, &w);
    //nextarg is a C string: next non-flag arg (.yo file)

    y86.reset();
    if (y86.load(nextarg)){
        y86.setTrace(f, d, e, m, w);

		uint64_t stop = 0;
		while (!stop){
        y86.clock();
        y86.trace();
        stop = y86.getStat();
		}
    }
    y86.dumpProcessorRegisters();
    y86.dumpProgramRegisters();
    y86.dumpMemory();    

}
char* getOptions(int argc, char *argv[], bool *pf, bool *pd, bool *pe, bool *pm, bool *pw)
{
	int c = 0;
	while ((c = getopt (argc, argv, "fdemw::")) != -1)
	{
		switch (c)
		{
			case 'f':
				*pf = true;
				break;
			case 'd':
				*pd = true;
				break;
			case 'e':
				*pe = true;
				break; 
			case 'm':
				*pm = true;
				break;
			case 'w':
				*pw = true;
				break;
			default:
				abort();
		} 
	}
	
    return argv[optind];
}
void usage(void)
{
    printf("Usage: yess <objectfile>\n"
          "<objectfile> must be created with the Y86 assembler 'yas' (has .yo extension.)\n");
    exit(0);
}

