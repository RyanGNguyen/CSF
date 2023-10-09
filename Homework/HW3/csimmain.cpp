#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

#include "csimfuncs.h"


int main(unsigned argc, char* argv[]) {
    //Check if arguments are valid
    int err = checkArgs(argc, argv); 
    if (err > 0) {
        std::cerr << "Invalid Arguments" << std::endl;
        return err; 
    }

    std::string filename = argv[8];     // Unsure if this is correct
    std::ifstream tracefile(filename);  
    std::string buffer;

    if (tracefile.is_open()) {
        unsigned int total_loads = 0;
        unsigned int total_stores = 0;
        unsigned int load_hits = 0;
        unsigned int load_misses = 0;
        unsigned int store_hits = 0;
        unsigned int store_misses = 0;
        unsigned int total_cycles = 0;

        while (tracefile) {
            std::getline (tracefile, buffer);   // can replace with get()?
            // Read in s or l 
            // Read in address
            // Read in size; not important for milestone 1
        }
    }
    return 0;
}