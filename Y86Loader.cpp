#include "Y86Loader.h"
#include "Y86.h"
	Y86Loader::Y86Loader(){
		
	}
	
    bool Y86Loader::isValidFileName(string name)
    {
	  int strLen = name.length();
	  if (name[strLen - 3] == '.' && name[strLen - 2] == 'y' && name[strLen - 1] == 'o')
	  {
		  return true;
	  }
	  return false; 
    }

	bool Y86Loader::checkLine(string line)
    {
      if (checkPipe(line))
	  {
		if (isBlankLine(line))
			return true;
		if (isCommentLine(line))
		{
			if (hasNoAddress(line))
				return true;
			if (hasValidAddress(line))
				return true;
		}  
		if (hasData(line) && hasValidAddress(line))
			return true;
	  }
	  return false;
    }
	bool Y86Loader::hasNoAddress(string line)
	{
		for (int i = 0; i <= 7; i++)
		{
			if (line[i] != ' ')
				return false;
		}
		return true;
	}
	bool Y86Loader::checkPipe(string line)
	{
		if (hasValidAddress(line))
		{
			if (line.at(5) == ':' && line.at(28) == '|')
				return true;
			else if (line.at(6) == ':' && line.at(29) == '|')
				return true;
			else
				return false;
		}
		else
		{
			if (line.at(29) == '|' || line.at(28) == '|')
				return true;
		}
		return false;
	}
    bool Y86Loader::hasValidAddress(string address)
    {
    	string hex = "0x";
    	if (address.substr(0, 2).compare("0x") == 0)
        {
      	  if (address[5] == ':')
          {
        	if (((address[2] >= '0' && address[2] <= '9') || (address[2] >= 'a' && address[2] <= 'f')) && 
          		((address[3] >= '0' && address[3] <= '9') || (address[3] >= 'a' && address[3] <= 'f')) && 
              ((address[4] >= '0' && address[4] <= '9') || (address[4] >= 'a' && address[4] <= 'f')))
            {
          	  return true;
            }
        }
        if (address[6] == ':') 
        {
        	if ((address[2] >= '0' && address[2] <= '2') && ((address[3] >= '0' && address[3] <= '9') || 
          		(address[3] >= 'a' && address[3] <= 'f')) && ((address[4] >= '0' && address[4] <= '9') || 
          		(address[4] >= 'a' && address[4] <= 'f')) && ((address[5] >= '0' && address[5] <= '9') || 
          		(address[5] >= 'a' && address[5] <= 'f')))
            {
            	return true; 
            }
        }
      }
      return false;
    }
	
	bool Y86Loader::hasValidAddressNumber(string line, uint64_t *addressCounter)
	{
		uint64_t found = line.find(".pos");
		uint64_t foundAlign = line.find(".align");
		
		if (getAddress(line) !=  0 ){
			if(*addressCounter != getAddress(line)){
				if (found == std::string::npos){
					if (foundAlign == std::string::npos){
						return false;
					} 
				}
			}
		}
		if (found != std::string::npos){
			*addressCounter = getAddress(line);
		}
		else if (foundAlign != std::string::npos){
			*addressCounter = getAddress(line);
		}
		else{
			*addressCounter += hasValidData(line); 
		}
		return true;
	}
    bool Y86Loader::isCommentLine(string line)
    {
      int i = 0;
	  if (line[5] == ':')
	  {
		  i = 6;
	  }
	  if (line[6] == ':')
	  {
		  i = 7;
	  }
      while (line[i] == ' ')
      {
      		i++;
      }
      if (line[i] == '|') 
      {
		  return true;
	  }
      return false;
    }
    bool Y86Loader::isBlankLine(string line)
    {
	  bool pipe = false;
	  unsigned int i = 0; 
      for (i = 0; i < line.length(); i++)
      {
        if (line[i] == '|')
        {
        	pipe = true;
        }
      	if (line[i] != ' ' && line[i] != '|') 
        {
        	return false; 
        }
      }
	  if (pipe)
	  {
		  return true;
	  }
      return false;
    }
    //checks for valid data
    bool Y86Loader::hasData(string line)
    {
		int val = hasValidData(line);
    	if(val != 0)
		{
			return true;
		}
        return false;
    }
	
	int Y86Loader::hasValidData(string line) 
	{
		unsigned int i;
		for (i = 7; line[i] != ' '; i++);
		string data = line.substr(7, i - 7);
		
		if (hasValidAddress(line))
		{
			for (i = 0; i < data.length(); i++)
			{
				if (!checkNumber(data[i]))
					return 0;
			}
		}
		if (data.length() % 2 != 0)
			return 0; 
			 
		return data.length() / 2;
	}
	
	//checks to make sure the given char is valid
    bool Y86Loader::checkNumber(char val)
	{
		if ((val >= '0' && val <= '9') || (val >= 'a' && val <= 'f') ||
				(val >= 'A' && val <= 'F') )
		{
			return true;
		}
		return false; 
	}
    
    uint64_t Y86Loader::getAddress(string line)
    {
	  string nums = "";
      if (hasValidData(line) || isCommentLine(line))
      {
      	if (line[5] == ':')
			nums = line.substr(2, 3);
        else
			nums = line.substr(2, 4);
      }
      int hexVal = 0;   
      stringstream hexNum;
	  hexNum << hex << nums;
	  hexNum >> hexVal;
      return hexVal; 
    }
	
	uint64_t Y86Loader::getData(string line)
	{
		string sData = "";
		int numBytes = hasValidData(line);
		
		if (hasValidData(line) || isCommentLine(line))
		{
			sData = line.substr(7, numBytes * 2);
		}
		uint64_t data = 0;
    	stringstream ssData;
		ssData << hex << sData;
		ssData >> data;
		return data;
	}
	uint64_t Y86Loader::getDataSplit(string line)
	{
		uint64_t data = 0;
    	stringstream ssData;
		ssData << hex << line;
		ssData >> data;
		return data;
	}

    void Y86Loader::storeData(string line, Memory &mem)
    {		
		uint64_t address = Y86Loader::getAddress(line);
		int bytes = hasValidData(line);
		if (bytes > 8)
		{
			Y86Loader::storeDataSplit(line, address, bytes, mem);
		}
		else
		{
			uint64_t data = 0;
			string sData = "";
		
			if (hasValidData(line) || isCommentLine(line))
			{
				sData = line.substr(7, bytes * 2);
			}
			data = Y86Loader::getData(line);
			Y86Loader::storeByte(data, address + (bytes - 1), bytes, mem);
		}
		
    }
	void Y86Loader::storeDataSplit(string line, uint64_t address, int bytes, Memory &mem)
	{
		uint64_t data = 0;
		
		if (bytes == 9)
		{
			string temp = line.substr(7, 16);
			string tempTwo = line.substr(23, 2);
			data = Y86Loader::getDataSplit(tempTwo);
			Y86Loader::storeByte(data, address + (bytes - 1), 1, mem); 
			data = Y86Loader::getDataSplit(temp); 
			Y86Loader::storeByte(data, address + (bytes - 2), 8, mem);
			
		}
		if (bytes == 10) 
		{
			string temp = line.substr(7, 16);
			string tempTwo = line.substr(23, 4); 
			data = Y86Loader::getDataSplit(tempTwo);
			Y86Loader::storeByte(data, address + (bytes - 1), 2, mem); 
			data = Y86Loader::getDataSplit(temp); 
			Y86Loader::storeByte(data, address + (bytes - 3), 8, mem);		
		}
	}
    void Y86Loader::storeByte(uint64_t data, uint64_t address, int bytes, Memory &mem)
    {
		if(bytes == 0)
            return; 
        else
        {
            Y86Loader::storeByte(data >> 8, address - 1, bytes - 1, mem);
            data &= 0xff;  
			mem.putByte(address, data);		
        } 
    }
