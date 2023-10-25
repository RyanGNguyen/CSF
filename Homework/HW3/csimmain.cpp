#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <isstream>
#include "csimfuncs.h"

int main(int argc, char* argv[]) {
    // Check if arguments are valid
    checkArgs(argc, argv); 

    // Initialize cache
    Cache cache(argv); 
    
    std::string line;
    while (getline(cin, line)){
        istringstream iss(line);               // Convert each line into a stringstream
        vector<string> v; 
        string word;   
        for (unsigned i = 0; i < 3; i++) {       // Divide each line into 3 words
            iss >> word; 
            v.push_back(word);                   // Store each word in a vector 
        }
        Cache.runTrace(v[0], stoul(v[1], NULL, 16));             // Run trace
    }
    cache.print_statistics();                   // Print statistics

    return 0;
}