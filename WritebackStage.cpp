
#include "Y86.h"
#include "WritebackStage.h"


void WritebackStage::reset(ProgRegisters *pregs, Forward *pforward)
{
	regs = pregs;
	forward = pforward;
	stat.setInput(SBUB);
    icode.setInput(INOP);
    ifun.setInput(FNONE);
    valE.setInput(0);
    valM.setInput(0);
    dstE.setInput(RNONE);
    dstM.setInput(RNONE);
}

void WritebackStage::updateWRegister(uint64_t pstat, uint64_t picode, uint64_t pifun, uint64_t pvalE, 
										uint64_t pvalM, uint64_t pdstE, uint64_t pdstM)
{
	stat.setInput(pstat);
    icode.setInput(picode);
    ifun.setInput(pifun);
    valE.setInput(pvalE);
    valM.setInput(pvalM);
    dstE.setInput(pdstE);
    dstM.setInput(pdstM);
}

void WritebackStage::clockP0()
{
	if (stageStall){
		stageStall = false;
	}
	else{
		stat.clock();
		icode.clock();
		ifun.clock();
		valE.clock();
		valM.clock();
		dstE.clock();
		dstM.clock();
	}
		w_dstE = dstE.getState();
		w_dstM = dstM.getState();
		w_valE = valE.getState();
		w_valM = valM.getState();

		forward->setW_dstE(w_dstE);
		forward->setW_dstM(w_dstM);
		forward->setW_valE(w_valE);
		forward->setW_valM(w_valM);
		forward->setw_stat(stat.getState());
		forward->setW_icode(icode.getState());
	
    
}

void WritebackStage::clockP1()
{
	regs->setReg(w_dstE, w_valE);
	regs->setReg(w_dstM, w_valM);
	
	
}

