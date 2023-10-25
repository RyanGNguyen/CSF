#ifndef CSIMFUNCS_H
#define CSIMFUNCS_H

#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <list>
#include <map>

struct Slot {
  unsigned tag;
  bool dirty = false; 
};
/* 
struct Set {
  std::vector<Slot> slots;
};
*/

struct Set {
    std::list<Slot> slots; // list of all slots in a set
    std::map<unsigned, std::list<Slot>::iterator> slot_tag_map; // map of tag to iterator of a slot
};
/*
struct Cache {
  std::vector<Set> sets;
};
*/

class Cache {
  public:
    unsigned int numSets;
    unsigned int numBlocks;
    unsigned int numBytes;
    bool allocate_or_no; // true = write allocate, false = no write allocate
    bool through_or_back; // true = write through, false = write back
    bool lru_fifo; // true = lru, false = fifo

    unsigned int total_loads = 0;
    unsigned int total_stores = 0;
    unsigned int load_hits = 0;
    unsigned int load_misses = 0;
    unsigned int store_hits = 0;
    unsigned int store_misses = 0;
    unsigned int total_cycles = 0;

    std::vector<Set> sets;

    Cache(char* parameters[]);

    unsigned int get_tag(std::uint32_t address);
    unsigned int get_index(std::uint32_t address);

    bool find_hit(unsigned int index, unsigned int tag);

    void runTrace(char instruction, uint32_t address);
    void load(uint32_t address);
    void store(uint32_t address);
    void add_slot(unsigned int index, unsigned int tag);
    void print_statistics();
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

#endif // CSIMFUNCS_H