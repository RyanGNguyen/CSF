#ifndef CSIMFUNCS_H
#define CSIMFUNCS_H

#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

struct Slot {
  unsigned tag;
  bool valid = false; 
  bool dirty = false; 
  double load_ts, access_ts; 
};

struct Set {
  std::vector<Slot> slots;
};

struct Cache {
  std::vector<Set> sets;
};

// Checks if the arguments are valid
void checkArgs(int argc, char* argv[]);

// Check for correct number of arguments
void checkArgc(unsigned argc);
    
// Check if number of sets is valid
void checkNumSets(std::string arg1);

// Check if number of blocks is valid
void checkNumBlocks(std::string arg2);

// Check if number of bytes is valid
void checkNumBytes(std::string arg3); 

// Check if write-allocate is valid
void checkWriteAlloc(std::string arg4);

// Check if write-through is valid
void checkWriteThrough(std::string arg5);

// Check write-back and no-write-allocate
void checkWriteBackAndNoAlloc(std::string arg4, std::string arg5);

// Check eviction policy
void checkEviction(std::string arg2, std::string arg6);

// Get tag bits
std::uint32_t get_tag(std::uint32_t address, int numSets, int blockBits);

// Get set bits
std::uint32_t get_set(std::uint32_t address,int numSets, int blockBits);

#endif // CSIMFUNCS_H