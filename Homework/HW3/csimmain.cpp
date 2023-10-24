#include <sstream>
#include <chrono>
#include <ctime>    

#include "csimfuncs.h"

unsigned int total_loads = 0;
unsigned int total_stores = 0;
unsigned int load_hits = 0;
unsigned int load_misses = 0;
unsigned int store_hits = 0;
unsigned int store_misses = 0;
unsigned int total_cycles = 0;

std::time_t const start_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); 

int main(int argc, char* argv[]) {
    //Check if arguments are valid
    checkArgs(argc, argv); 

    unsigned numSets = std::stoul(argv[1], NULL, 10); 
    unsigned numBlocks = std::stoul(argv[2], NULL, 10);
    unsigned numBytes = std::stoul(argv[3], NULL, 10);
    std::string writeAlloc = argv[4];
    std::string writeThrough = argv[5];
    std::string eviction = argv[6];

    Cache cache;                                   // Initialize cache 
    cache.sets.resize(numSets);
    for (unsigned i = 0; i < numSets; i++) {
        cache.sets[i].slots.resize(numBlocks);
    }
    
    std::string fileName;
    fileName << std::cin; 
    std::ifstream traceFile(fileName);  

    if (traceFile.is_open()) {    
        std::string buffer;
        while (std::getline(traceFile, buffer)) {     // Read each line from file into a buffer string
            std::istringstream iss(buffer);           // Convert buffer string into a stringstream
            std::vector<std::string> v; 
            std::string word;   
            for (unsigned i = 0; i < 3; i++) {       // Divide each line into 3 words
                iss >> word; 
                v.push_back(word);                   // Store each word in a vector 
            }                 
            // Address check
            int num_block_bits = log2(numBlocks);
            
            unsigned n = std::stoi(v[1], NULL, 16);        // Convert hex address to unsigned int
            int set_tags = get_set(n, numSets, num_block_bits); // Get set bits
            std::bitset<32> address{n};                    // Convert unsigned int to binary
            int set_bits = get_set(n, numSets, num_block_bits); // Get set bits
            int tag_bits = get_tag(n, numSets, num_block_bits); // Get tag bits

            cache.sets[set_tags].slots[tag_bits].tag = tag_bits;
            cache.sets[set_tags].slots[tag_bits].valid = true;
            auto const now = std::chrono::system_clock::now();

            // Store/Load check 
            if (v[0].compare("l") == 0) {         // If word is "l", update load info
                cache.sets[set_tags].slots[tag_bits].load_ts = std::chrono::system_clock::to_time_t(now) - start_t; 
                total_loads++;
            } else if (v[0].compare("s") == 0) {  // If word is "s", update store info
                cache.sets[set_tags].slots[tag_bits].access_ts = std::chrono::system_clock::to_time_t(now) - start_t; 
                total_stores++; 
            }
//test msg
            // Read in size 
        }
    }
    return 0;
}