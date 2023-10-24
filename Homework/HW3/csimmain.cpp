#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <bitset>
#include <vector>
#include <cmath>

#include "csimfuncs.h"


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

    unsigned int total_loads = 0;
    unsigned int total_stores = 0;
    unsigned int load_hits = 0;
    unsigned int load_misses = 0;
    unsigned int store_hits = 0;
    unsigned int store_misses = 0;
    unsigned int total_cycles = 0;
   
    std::ifstream traceFile(filename);  

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
            // Store/Load check 
            if (v[0].compare("l") == 0) {         // If word is "l", increment total loads
                total_loads++;
            } else if (v[0].compare("s") == 0) {  // If word is "s", increment total loads
                total_stores++; 
            }
            unsigned n = std::stoi(v[1], NULL, 16);        // Convert hex address to unsigned int
            std::bitset<32> address{n};                    // Convert unsigned int to binary
            std::string bitString = address.to_string();   // Convert binary to string 

            std::string tag = bitString.substr(0, log2(numBlocks));                   
            std::string index = bitString.substr(log2(numBlocks), log2(numSets));
            //std::string offset = bitString.substr(log2(numBlocks) + log2(numSets), log2(numBytes));
            
            unsigned t = std::stoul(tag, NULL);
            unsigned i = std::stoul(index, NULL);

            cache.sets[i].slots[t].tag = t; 

            // Read in size 
        }
    }
    return 0;
}