#include "ProgRegisters.h"
#include "Memory.h"
#include "Y86.h"

Y86::Y86()
{
    reset();
}
void Y86::reset()
{
    Y86::memory.reset();
    Y86::regs.reset();
	stage[0] = &fetchStage;
	stage[1] = &decodeStage;
	stage[2] = &executeStage;
	stage[3] = &memoryStage;
	stage[4] = &writebackStage;
	
	fetchStage.reset(&decodeStage, &memory, &forward);
	decodeStage.reset(&executeStage, &regs, &forward);
	executeStage.reset(&memoryStage, &regs, &forward);
	memoryStage.reset(&writebackStage, &memory, &regs, &forward);
	writebackStage.reset(&regs, &forward);
	
	cycles = 0;
}

void Y86::clock()
{
    regs.clock();
	for(int i = 0; i < 5; i++)
	{
		stage[i]->clockP0();
	}
	for(int i = 0; i < 5; i++)
	{
		stage[i]->clockP1();
	}
	
	pipeControl();
}

uint64_t Y86::getStat()
{
	uint64_t wbStat = writebackStage.getStat();
	if (wbStat == SAOK || wbStat == SBUB)
		return 0;
	return wbStat;
}

bool Y86::load(char *fname)		// takes the name of an input file 
{  
	//checking to see if fileName is valid
	bool fileName = Y86::loader.isValidFileName(fname);
	if (!fileName)
	{
		cout << "File open failed: <" << (string)fname << ">" << endl;
		cout << "Usage: yess <" << (string)fname << ">.yo" << endl;
		return false;
	}
	
	int lineCount = 1;
	bool lineCheck = false;
	bool addressCheck = false;
	uint64_t counter = 0;
	uint64_t *addressCounter = &counter; 
	bool addressAlreadyOffTrack = false;
	bool alreadyHaveError = false;
	ifstream inf;
	inf.open(fname, fstream::in);
	string line = "";
	int lineCountToError = 0;
	if (inf.is_open())
	{
		bool first = true;
		//while we still have lines to read check to see if each line is valid
		while(getline(inf, line))
		{
			lineCheck = Y86::loader.checkLine(line);
			addressCheck = Y86::loader.hasValidAddressNumber(line, addressCounter);
			if (!lineCheck || !addressCheck)
			{
				if (!alreadyHaveError && !addressAlreadyOffTrack){
					cout << "Error on line " << lineCount << endl;
					cout << line << endl; 
				} 
				
				if (first)
					lineCountToError = lineCount;
				first = false;
				if (!addressCheck)
					addressAlreadyOffTrack = true;
				if (!lineCheck)
					alreadyHaveError = true;
			}
			lineCount++; 
		}
		if (first)
			lineCountToError = lineCount; 
	}
	inf.close();
	
	//time to load data into simulated memory
	ifstream infTwo;
	infTwo.open(fname, fstream::in);
	string lineTwo = "";
	if (infTwo.is_open()){
		while(getline(infTwo, lineTwo) && lineCountToError > 1){
			//store the data from line to memory
			Y86Loader::storeData(lineTwo, Y86::getMemory());  
			lineCountToError--;
		}
	}
  if (alreadyHaveError || addressAlreadyOffTrack)
  	return false;
	return true;
}

void Y86::pipeControl(){
	//Should I stall or inject a bubble into Pipeline Register F?
	stage[0]->setBubble(0);
	if (((stage[2]->getIcode() == IMRMOVQ || stage[2]->getIcode() == IPOPQ) &&
	(executeStage.getDstM() == decodeStage.grabSrcA() || executeStage.getDstM() == decodeStage.grabSrcB())) ||
	(IRET == stage[1]->getIcode() || IRET == stage[2]->getIcode() || IRET == stage[3]->getIcode())) {   
		 stage[0]->setStall(1); 
	}
	 
	//Should I stall or inject a bubble into Pipeline Register D?
	 if ((stage[2]->getIcode() == IMRMOVQ || stage[2]->getIcode() == IPOPQ) && 
	 (executeStage.getDstM() == decodeStage.grabSrcA() || executeStage.getDstM() == decodeStage.grabSrcB())){
		 stage[1]->setStall(1);
	 }
	 
	 if ((((stage[2]->getIcode() == IJXX) && !(executeStage.getCnd())) && stage[3]->getStat() != 2) ||
	 (!((stage[2]->getIcode() == IMRMOVQ || stage[2]->getIcode() == IPOPQ) && 
	 (executeStage.getDstM() == decodeStage.grabSrcA() || executeStage.getDstM() == decodeStage.grabSrcB())) &&
	 (IRET == stage[1]->getIcode() || IRET == stage[2]->getIcode() || IRET == stage[3]->getIcode() ))){
		 stage[1]->setBubble(1);
	 }  
	 
	//Should I stall or inject a bubble into Pipeline Register E?
	stage[2]->setStall(0);
	if ((((stage[2]->getIcode() == IJXX) && !(executeStage.getCnd())) && stage[3]->getStat() != 2) ||
	 ((stage[2]->getIcode() == IMRMOVQ || stage[2]->getIcode() == IPOPQ) && 
	 (executeStage.getDstM() == decodeStage.grabSrcA() || executeStage.getDstM() == decodeStage.grabSrcB())) ){
		 stage[2]->setBubble(1);
	 } 
	
	//Should I stall or inject a bubble into Pipeline Register M?
	stage[3]->setStall(0);
	if ((stage[3]-> getStat() == SADR || stage[3]-> getStat() == SINS || 
	stage[3]-> getStat() == SHLT) || (stage[4]-> getStat() == SADR ||
	stage[4]-> getStat() == SINS || stage[4]-> getStat() == SHLT)){
		stage[3]->setBubble(1);
	}  
	
	
	//Should I stall or inject a bubble into Pipeline Register W?
	stage[4]->setBubble(0);
	if(stage[4]->getStat() == SADR || stage[4]->getStat() == SINS || 
	stage[4]->getStat() == SHLT ){
		stage[4]->setStall(1);  
	}
}  
