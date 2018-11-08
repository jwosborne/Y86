
CXX = g++
CXXFLAGS = -Wall -g -std=c++0x
STAGEDOTH = PipeStage.h FetchStage.h DecodeStage.h ExecuteStage.h MemoryStage.h WritebackStage.h


# Put object files in next line
OBJS = Memory.o ProgRegisters.o Y86.o Sim.o Y86Loader.o Y86trace.o FetchStage.o DecodeStage.o ExecuteStage.o MemoryStage.o WritebackStage.o 
# The next line is same as 'OBJS' line above except replace Sim.o with testSim.o 

OBJTEST = Memory.o ProgRegisters.o Y86.o testSim.o Y86Loader.o Y86trace.o FetchStage.o DecodeStage.o ExecuteStage.o MemoryStage.o WritebackStage.o
 
yess:	$(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o yess

ytest:  $(OBJTEST)
	$(CXX) $(CXXFLAGS) $(OBJTEST)  -o ytest

#your object file targets below here...

Memory.o: Memory.cpp Memory.h Sim.h

ProgRegisters.o: ProgRegisters.cpp ProgRegisters.h Register.h Sim.h

Y86.o: Y86.cpp Y86.h Forward.h $(STAGEDOTH) Y86Loader.h Memory.h ProgRegisters.h

Sim.o: Sim.cpp Sim.h Memory.h ProgRegisters.h Y86.h

Y86Loader.o: Y86Loader.cpp Y86Loader.h Memory.h Sim.h

Y86trace.o: Y86trace.cpp $(STAGEDOTH) Y86.h Sim.h

FetchStage.o: FetchStage.cpp FetchStage.h PipeStage.h Forward.h ExecuteStage.h Memory.h DecodeStage.h Register.h Sim.h Y86.h

DecodeStage.o: DecodeStage.cpp DecodeStage.h PipeStage.h Forward.h Sim.h Register.h DecodeStage.h ExecuteStage.h Y86.h

ExecuteStage.o:  ExecuteStage.cpp ExecuteStage.h PipeStage.h Forward.h ProgRegisters.h MemoryStage.h Register.h Sim.h Y86.h

MemoryStage.o: MemoryStage.cpp MemoryStage.h PipeStage.h Forward.h Memory.h WritebackStage.h Register.h Sim.h Y86.h

WritebackStage.o: WritebackStage.cpp  WritebackStage.h PipeStage.h Forward.h ProgRegisters.h Sim.h Register.h Y86.h


# clean target
clean:
		rm -f *.o yess
#		cp ../../../lab4/testSim.o testSim.o

sure:
		rm -f *.o yess
		make
