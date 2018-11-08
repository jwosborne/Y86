
#include "Y86.h"
#include "ExecuteStage.h"
#include "Forward.h"
#include "ProgRegisters.h"
#include <cmath>

void ExecuteStage::reset(MemoryStage *pmemory, ProgRegisters *pregs, Forward *pforward)
{
	memoryStage = pmemory;
	regs = pregs;
	forward = pforward;
	
	stat.setInput(SBUB); 
    icode.setInput(INOP);
    ifun.setInput(FNONE);
    valC.setInput(0);
    valA.setInput(0);
    valB.setInput(0);
    dstE.setInput(RNONE);
    dstM.setInput(RNONE);
    srcA.setInput(RNONE);
    srcB.setInput(RNONE);
}

void ExecuteStage::updateERegister(uint64_t pstat, uint64_t picode, uint64_t pifun, uint64_t pvalC, uint64_t pvalA, 
										uint64_t pvalB, uint64_t pdstE, uint64_t pdstM, uint64_t psrcA, uint64_t psrcB)
{
	stat.setInput(pstat);
    icode.setInput(picode);
    ifun.setInput(pifun);
    valC.setInput(pvalC);
    valA.setInput(pvalA);
    valB.setInput(pvalB);
    dstE.setInput(pdstE);
    dstM.setInput(pdstM);
    srcA.setInput(psrcA);
    srcB.setInput(psrcB); 
}

void ExecuteStage::clockP0()
{
	if (stageBubble){
		stageBubble = false;
		updateERegister(SBUB, INOP, FADDQ, 0, 0, 0, RNONE, RNONE, RNONE, RNONE);
	}

    stat.clock();
    icode.clock();
    ifun.clock();
    valC.clock();
    valA.clock();
    valB.clock();
    dstE.clock();
    dstM.clock();
    srcA.clock();
    srcB.clock();
	
	e_stat = stat.getState();
	e_icode = icode.getState();
	e_ifun = ifun.getState();
	e_valA = valA.getState();
    e_dstM = dstM.getState();
	
	getALUA();
	getALUB();
	getALUFunction();
	
	
	switch(aluFun)
	{
		case FADDQ:
			e_valE = aluA + aluB;
			break;
		case FSUBQ:
			e_valE = aluB - aluA;
			break;
		case FANDQ:
			e_valE = aluA & aluB;
			break;
		case FXORQ:
			e_valE = aluA ^ aluB;
			break;
		default:
			break;
	}

	if(setsCC())
	{
		if(isOverflow())
			of = 1;
        else
			of = 0;
        if(highBit(e_valE) == 1)
            sf = 1;
        else
			sf = 0;
        if(e_valE == 0)
			zf = 1;
        else
			zf = 0;
    }
	e_cnd = setCnd();
    selectDstE();
	
	forward->sete_valE(e_valE);
    forward->sete_dstE(e_dstE);
}

void ExecuteStage::clockP1()
{
	if(setsCC())
		setCCodes(zf, sf, of);
	uint64_t m_stat = forward->getm_stat();
	if(m_stat == SHLT || m_stat == SADR || m_stat == SINS)
	{
		e_stat = SBUB;
		e_icode = INOP;
		e_ifun = FADDQ;
		e_valE = 0;
		e_valA = 0;
		e_dstE = RNONE;
		e_dstM = RNONE;
        e_cnd = 0;
	}	 

	if(e_icode == IRRMOVQ && e_cnd == 0)
		e_dstE = RNONE;

	memoryStage->updateMRegister(e_stat, e_icode, e_ifun, e_valE, e_valA, e_dstE, e_dstM, e_cnd);
}

bool ExecuteStage::isOverflow()
{
	bool overflow = false;
	switch(aluFun)
	{
		case FADDQ:
            if(highBit(aluA) == highBit(aluB) && ((highBit(aluA) != highBit(e_valE)) || e_valE == 0))
            	overflow = true;
			break;
		case FSUBQ:	
			if((highBit(aluB) != highBit(aluA)) && (highBit(aluA) == highBit(e_valE)))
				overflow = true;
			break;
		default:
			break;
	}
	return overflow;
}

bool ExecuteStage::setCnd()
{
	int sF = regs->getCC(SF);
	int oF = regs->getCC(OF);
	int zF = regs->getCC(ZF);
	
	switch(e_ifun)
	{
		case LEQ:
			return (sF ^ oF) | zF;
		case LT:
			return sF ^ oF;
		case EQ:
			return zF;
		case NEQ:
			return !zF;
		case GTEQ:
			return !(sF ^ oF);
		case GT:
			return !(sF ^ oF) & !zF;
	}
	return true;
}

void ExecuteStage::setCCodes(int zF, int sF, int oF)
{
	regs->setCC(OF, oF);
	regs->setCC(SF, sF);
	regs->setCC(ZF, zF);
}

uint64_t ExecuteStage::highBit(uint64_t num)
{
    num >>= 63;
    num &= 1;
    return num;
}

void ExecuteStage::getALUA()
{
	if (e_icode == IRRMOVQ || e_icode == IOPX) 
		aluA = e_valA;
	else if (e_icode == IIRMOVQ || e_icode == IRMMOVQ || e_icode == IMRMOVQ)
		aluA = valC.getState();
	else if (e_icode == ICALL || e_icode == IPUSHQ)
		aluA = -8;
	else if (e_icode == IRET || e_icode == IPOPQ)
		aluA = 8;
	else
		aluA = 0;
}
void ExecuteStage::getALUB()
{
	if (e_icode == IRMMOVQ || e_icode == IMRMOVQ || e_icode == IOPX
		|| e_icode == ICALL || e_icode == IPUSHQ || e_icode == IRET
		|| e_icode == IPOPQ)
		aluB = valB.getState();
	else 
		aluB = 0;
}
void ExecuteStage::getALUFunction()
{
	if (e_icode == IOPX)
		aluFun = ifun.getState();
	else
		aluFun = FADDQ;
}
bool ExecuteStage::setsCC()
{
    uint64_t m_stat = forward->getm_stat();
    uint64_t w_stat = forward->getw_stat();
	if(e_icode == IOPX && (m_stat != SADR 
				&& m_stat != SINS 
				&& m_stat != SHLT) 
				&& (w_stat != SADR 
				&& w_stat != SINS 
				&& w_stat != SHLT))
		return true;
	return false;
}
void ExecuteStage::selectDstE()
{
	if (e_icode == IRRMOVQ && !e_cnd)
		e_dstE = RNONE;
	else
		e_dstE = dstE.getState();
}
