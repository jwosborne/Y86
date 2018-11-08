
#include "Y86.h"
#include "MemoryStage.h"


void MemoryStage::reset(WritebackStage *pwriteback, Memory *pmem, ProgRegisters *pregs, Forward *pforward)
{
	writeback = pwriteback;
	memory = pmem;
	regs = pregs;
	forward = pforward;
	
	stat.setInput(SBUB);
    icode.setInput(INOP);
    ifun.setInput(FNONE);
    valE.setInput(0);
    valA.setInput(0);
    dstE.setInput(RNONE);
    dstM.setInput(RNONE);
    cnd.setInput(false);
}

void MemoryStage::updateMRegister(uint64_t pstat, uint64_t picode, uint64_t pifun, uint64_t pvalE, 
							uint64_t pvalA, uint64_t pdstE, uint64_t pdstM, bool pcnd)
{
	stat.setInput(pstat); 
    icode.setInput(picode);
    ifun.setInput(pifun);
    valE.setInput(pvalE);
    valA.setInput(pvalA);
    dstE.setInput(pdstE);
    dstM.setInput(pdstM);
    cnd.setInput(pcnd);
}

void MemoryStage::clockP0()
{
	if (stageBubble){
		stageBubble = false;
		
		if (m_stat != SBUB){
			updateMRegister(SBUB, INOP, FADDQ, 0, 0, RNONE, RNONE, 0);
		}
		
	} 
	
    cnd.clock();
    stat.clock();
    icode.clock();
    ifun.clock();
    valE.clock();
    valA.clock();
    dstE.clock();
    dstM.clock();

	m_icode = icode.getState();
	m_ifun = ifun.getState();
	
    m_stat = stat.getState();
    m_dstE = dstE.getState();
    m_dstM = dstM.getState();
    m_valE = valE.getState();
	m_valA = valA.getState();
	
   	if(read())
		m_valM = getValue(calcAddress());
	else
		m_valM = 0;
	

	forward->setM_dstM(m_dstM);
    forward->setM_valM(m_valM);
    forward->setM_valE(m_valE);
    forward->setM_dstE(m_dstE);
	forward->setm_valA(m_valA);
    forward->setm_stat(m_stat);
	forward->setM_icode(icode.getState());
	forward->setM_Cnd(cnd.getState());

	if(memory->isError())
	{
		m_stat = SADR;
		m_valM = 0;
		forward->setm_stat(m_stat);
		forward->setM_valM(m_valM);
	}
}

void MemoryStage::clockP1()
{
	
	if(write())
		putValue(calcAddress(), m_valA);
	
	if(memory->isError())
	{
		m_stat = SADR;
		m_valM = 0;
		forward->setm_stat(m_stat);
		forward->setM_valM(m_valM);
	}	
	writeback->updateWRegister(m_stat, m_icode, m_ifun, m_valE, m_valM, m_dstE, m_dstM);
}

uint64_t MemoryStage::calcAddress()
{
	if(m_icode == IRMMOVQ || m_icode == IMRMOVQ || m_icode == IPUSHQ || m_icode == ICALL)
		return valE.getState();
	else if(m_icode == IPOPQ || m_icode == IRET)
		return m_valA;
	return 0;
}

uint64_t MemoryStage::getValue(uint64_t address)
{
    uint64_t value = 0;
	if(address > 10000)
		memory->getByte(address);
	for(uint64_t i = address + 7; i > address - 1; i--)
	{
        value <<= 8;
		value |= memory->getByte(i);
	}
    return value;
}

void MemoryStage::putValue(uint64_t address, uint64_t value)
{
    char byte = 0;
	if(address > 10000)
		memory->getByte(address);
    for(uint64_t i = address; i < address + 8; i++)
    {
        byte = value & 0xFF;
        memory->putByte(i, byte);
        value >>= 8;
    }
}

bool MemoryStage::read()
{
	if(m_icode == IMRMOVQ || m_icode == IPOPQ || m_icode == IRET)
		return true;
	return false;
}

bool MemoryStage::write()
{
	if(m_icode == IRMMOVQ || m_icode == IPUSHQ || m_icode == ICALL)
		return true;
	return false;
}

