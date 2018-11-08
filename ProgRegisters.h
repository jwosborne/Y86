
#ifndef PROGREGISTERS_H
#define PROGREGISTERS_H

#include "Sim.h"
#include "Register.h"
#include <cstdint>
#include <assert.h>

class ProgRegisters
{
    private:
        Register<uint64_t>   reg[NUM_REGISTERS];
        Register<uint64_t>   CC;

    public:
        ProgRegisters(void);   // constructor 
        void clock ();
        void setReg (unsigned regNum, uint64_t rval); 
        uint64_t getReg(unsigned regNum);
        void setCC(unsigned bitNum, unsigned val);
        unsigned getCC(unsigned bitNum);
        unsigned getCC();
        void reset (void);      
};
#endif
