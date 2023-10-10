#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <bitset>
#include <vector>

#include "csimfuncs.h"


int main(int argc, char* argv[]) {
    //Check if arguments are valid
    int err = checkArgs(argc, argv); 
    if (err > 0) {
        std::cerr << "Invalid Arguments" << std::endl;
        return err; 
    }

    unsigned numSets = std::stoul(argv[1], NULL, 10); 
    unsigned numBlocks = std::stoul(argv[2], NULL, 10);
    unsigned numBytes = std::stoul(argv[3], NULL, 10);
    std::string writeAlloc = argv[4];
    std::string writeThrough = argv[5];
    std::string filename = (argc == 8) ? argv[7] : argv[8];

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
            std::istringstream iss(buffer);           // Create a string stream from the buffer 
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
            std::bitset<32> address{v[1]};           // Convert address to binary bitset
            
            
            // Read in size 
        }
    }
    return 0;
}