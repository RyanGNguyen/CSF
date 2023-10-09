#ifndef CSIMFUNCS_H
#define CSIMFUNCS_H

#include <cstdlib>
#include <iostream>
#include <string>

// Checks if the arguments are valid
int checkArgs(unsigned argc, char* argv[]);

// Check for correct number of arguments
int checkArgc(unsigned argc);
    
// Check if number of sets is valid
int checkNumSets(std::string arg1);

// Check if number of blocks is valid
int checkNumBlocks(std::string arg2);

// Check if number of bytes is valid
int checkNumBytes(std::string arg3); 

// Check if write-allocate is valid
int checkWriteAlloc(std::string arg4);

// Check if write-through is valid
int checkWriteThrough(std::string arg5);

// Check write-back and no-write-allocate
int checkWriteBackAndNoAlloc(std::string arg4, std::string arg5);

// Check eviction policy
int checkEviction(std::string arg1, std::string arg2, std::string arg4, std::string arg6); 

#endif // CSIMFUNCS_H