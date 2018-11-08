
#include "Y86.h"

void DecodeStage::reset(ExecuteStage *pexecute, ProgRegisters *pregs, Forward *pforward)
{
	executeStage = pexecute;
	regs = pregs;
	forward = pforward;
	
	stat.setInput(SBUB);
    icode.setInput(INOP);
    ifun.setInput(FNONE);
    rA.setInput(RNONE);
    rB.setInput(RNONE);
    valC.setInput(0);
    valP.setInput(0);
}

void DecodeStage::updateDRegister(uint64_t pstat, uint64_t picode, uint64_t pifun, uint64_t prA, 
									uint64_t prB, uint64_t pvalC, uint64_t pvalP)
{
	stat.setInput(pstat);
    icode.setInput(picode);
    ifun.setInput(pifun);
    rA.setInput(prA);
    rB.setInput(prB);
    valC.setInput(pvalC);
    valP.setInput(pvalP);
}

void DecodeStage::clockP0()
{
	if (stageStall){
		stageStall = false;
	}
	else if (stageBubble){
		stageBubble = false;
		updateDRegister(SBUB, INOP, FADDQ, RNONE, RNONE, 0, 0);
		stat.clock();
		icode.clock();
		ifun.clock();
		rA.clock(); 
		rB.clock();
		valC.clock();
		valP.clock();  
	}
	else{
		stat.clock();
		icode.clock();
		ifun.clock();
		rA.clock();
		rB.clock();
		valC.clock();
		valP.clock();
	}
	
}

void DecodeStage::clockP1()
{
	d_stat = stat.getState();
	d_icode = icode.getState();
	d_ifun = ifun.getState();
    d_valC = valC.getState();
	d_rA = rA.getState();
	d_rB = rB.getState();
    getSrcA();
    getSrcB();
    getDstE();
    getDstM();
	

	if(d_icode == IPOPQ || d_icode == IRET)
		d_rA = RSP;
	if(d_icode == IPUSHQ || d_icode == IPOPQ || d_icode == ICALL || d_icode == IRET)
		d_rB = RSP;                     // set rB to the address of %rsp  
	
    selectFwdA();
    forwardB();
	if (stat.getState() == SBUB)
	{
		d_dstE = RNONE;
		d_dstM = RNONE;
		d_srcA = RNONE;
		d_srcB = RNONE;
	}
	executeStage->updateERegister(d_stat, d_icode, d_ifun, d_valC, d_valA, d_valB, d_dstE, d_dstM, d_srcA, d_srcB);
}

void DecodeStage::getSrcA()
{
    if(d_icode == IRRMOVQ || d_icode == IRMMOVQ || d_icode == IOPX || d_icode == IPUSHQ)
        d_srcA = d_rA;
    else if(d_icode == IPOPQ || d_icode == IRET)
        d_srcA = RSP;
    else
        d_srcA = RNONE;
}

void DecodeStage::getSrcB()
{
    if(d_icode == IOPX || d_icode == IRMMOVQ || d_icode == IMRMOVQ)
        d_srcB = d_rB;
    else if(d_icode == IPUSHQ || d_icode == IPOPQ || d_icode == ICALL || d_icode == IRET)
        d_srcB = RSP;
    else
        d_srcB = RNONE;
}

void DecodeStage::getDstE()
{
    if(d_icode == IRRMOVQ || d_icode == IIRMOVQ || d_icode == IOPX)
        d_dstE = d_rB;
    else if(d_icode == IPUSHQ || d_icode == IPOPQ || d_icode == ICALL || d_icode == IRET)
        d_dstE = RSP;
    else
        d_dstE = RNONE;
}

void DecodeStage::getDstM()
{
    if(d_icode == IMRMOVQ || d_icode == IPOPQ)
        d_dstM = d_rA;
    else
        d_dstM = RNONE;
}
void DecodeStage::selectFwdA()
{
   if(d_icode == ICALL || d_icode == IJXX)
       d_valA = valP.getState();
   else if(d_srcA == forward->gete_dstE())
	   d_valA = forward->gete_valE();
   else if(d_srcA == forward->getM_dstM())
	   d_valA = forward->getM_valM();
   else if(d_srcA == forward->getM_dstE())
	   d_valA = forward->getM_valE();
   else if(d_srcA == forward->getW_dstM()) 
	   d_valA = forward->getW_valM();
   else if(d_srcA == forward->getW_dstE())
       d_valA = forward->getW_valE();			
   else
       if (d_rA == RNONE)
           d_valA = 0;
       else
           d_valA = regs->getReg(d_rA);
}

void DecodeStage::forwardB()
{
	if(d_srcB == forward->gete_dstE())
		d_valB = forward->gete_valE();
	else if(d_srcB == forward->getM_dstM())
		d_valB = forward->getM_valM();
	else if(d_srcB == forward->getM_dstE())
		d_valB = forward->getM_valE();
	else if(d_srcB == forward->getW_dstM())
		d_valB = forward->getW_valM();
    else if(d_srcB == forward->getW_dstE())
        d_valB = forward->getW_valE();
    else
        if (d_rB == RNONE)
            d_valB = 0;
        else
            d_valB = regs->getReg(d_rB); // = registerFile d_rvalB
}

//void DecodeStage::trace();
