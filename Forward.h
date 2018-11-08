/*
    File:   Forward.h
    Desc:   Declares Forward class and associated constants
*/

#ifndef FORWARD_H
#define FORWARD_H

class Forward
{
	uint64_t W_dstE;
	uint64_t W_valE;
    uint64_t W_dstM;
    uint64_t W_valM;
    uint64_t M_dstE;
    uint64_t M_valE;
    uint64_t M_dstM;
    uint64_t M_valM;
	uint64_t m_valA;
    uint64_t e_dstE;
    uint64_t e_valE;
	
	uint64_t m_stat;
	uint64_t M_icode;
	bool M_Cnd;
	uint64_t w_stat;
	uint64_t W_icode;
	
	public:
		uint64_t getW_dstE() {return W_dstE;}
		void setW_dstE(uint64_t dstE) {W_dstE = dstE;}
		
		uint64_t getW_valE() {return W_valE;}
		void setW_valE(uint64_t valE) {W_valE = valE;}
		
		uint64_t getW_dstM() {return W_dstM;}
		void setW_dstM(uint64_t dstM) {W_dstM = dstM;}
		
		uint64_t getW_valM() {return W_valM;}
		void setW_valM(uint64_t valM) {W_valM = valM;}
		
		uint64_t getM_dstE() {return M_dstE;}
		void setM_dstE(uint64_t dstE) {M_dstE = dstE;}
		
		uint64_t getM_valE() {return M_valE;}
		void setM_valE(uint64_t valE) {M_valE = valE;}
		
		uint64_t getM_dstM() {return M_dstM;}
		void setM_dstM(uint64_t dstM) {M_dstM = dstM;}
		
		uint64_t getM_valM() {return M_valM;}
		void setM_valM(uint64_t valM) {M_valM = valM;}
		
		uint64_t getm_valA() {return m_valA;}
		void setm_valA(uint64_t valA) {m_valA = valA;}
		
		uint64_t gete_dstE() {return e_dstE;}
		void sete_dstE(uint64_t dstE) {e_dstE = dstE;}
		
		uint64_t gete_valE() {return e_valE;}
		void sete_valE(uint64_t valE) {e_valE = valE;}
		
		uint64_t getm_stat() {return m_stat;}
		void setm_stat(uint64_t stat) {m_stat = stat;}
		
		uint64_t getw_stat() {return w_stat;}
		void setw_stat(uint64_t stat) {w_stat = stat;}
		
		uint64_t getM_icode() {return M_icode;}
		void setM_icode(uint64_t icode) {M_icode = icode;}
		
		uint64_t getW_icode() {return W_icode;}
		void setW_icode(uint64_t icode) {W_icode = icode;}
		
		uint64_t getM_Cnd() {return M_Cnd;}
		void setM_Cnd(uint64_t cnd) {M_Cnd = cnd;}
};

#endif
