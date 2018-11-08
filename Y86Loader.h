
#ifndef Y86LOADER_H
#define Y86LOADER_H
#include <iostream>
#include <string>
#include <sstream>
#include <fstream> 
#include "Sim.h" 
#include <fstream>
#include "Memory.h"

 
using namespace std;

class Y86Loader
{
private:
	

public: 
	Y86Loader();
    void load(char *argv);  
	static uint64_t getDataSplit(string line);
	static void storeDataSplit(string line, uint64_t address, int bytes, Memory &mem); 
    static void readFile(ifstream file);
	static bool isFileValid(ifstream file, string name);
	static bool isBlankLine(string line); 
    static bool isValidFileName(string name);
    static bool checkLine(string line);
    static bool hasValidAddress(string address);
	static bool hasValidAddressNumber(string address, uint64_t *addressCounter);
    static bool isCommentLine(string line); 
	static bool hasNoAddress(string line);
	static bool checkNumber(char val);
    static bool hasData(string line);
    static int hasValidData(string line);
	static bool checkPipe(string line);
    static uint64_t getAddress(string line);
	static uint64_t getData(string line);
    static bool hasSpaces(string data, int start, int end);
    static bool checkHex(string data, int start, int end);
    static void storeData(string line, Memory &mem); 
    static void storeByte(uint64_t data, uint64_t address, int bytes, Memory &mem);
};

#endif