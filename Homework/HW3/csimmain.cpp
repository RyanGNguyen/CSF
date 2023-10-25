#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <isstream>
#include "csimfuncs.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Check if arguments are valid
    checkArgs(argc, argv); 

    // Initialize cache
    Cache cache(argv); 
    
    string line;
    char operation;
    uint32_t address;
    while (getline(cin, line)){
        istringstream iss(line);           // Convert buffer string into a stringstream
        vector<string> v; 
        string word;   
        for (unsigned i = 0; i < 3; i++) {       // Divide each line into 3 words
            iss >> word; 
            v.push_back(word);                   // Store each word in a vector 
        }
    }
    std::string fileName;
    std::cin >> fileName; 
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

            // Find Slot
            int num_block_bits = log2(numBlocks);     // Calculate number of block bits
            unsigned n = std::stoi(v[1], NULL, 16);        // Convert hex address to unsigned int
            int set_bits = get_set(n, numSets, num_block_bits); // Get set bits 
            int tag_bits = get_tag(n, numSets, num_block_bits); // Get tag bits
            Slot cur_slot = cache.sets[set_bits].slots[tag_bits]; // Get slot from cache
            std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); // Get current time
            if (eviction.compare("lru") == 0) {
                cur_slot.access_ts = difftime(now, start_t); // Update time when block was accessed
            }
            total_cycles++;

            // Store/Load check 
            if (v[0].compare("l") == 0) {       
                total_loads++;
                cur_slot.load_ts = difftime(now, start_t);  // Update time when block was loaded into slot
                if (cur_slot.valid == false) {   // Check if load is a miss
                    load_misses++;
                    // Loading from memory 
                    total_loads++; 
                    total_cycles += 100;
                    // Update cache
                    cur_slot.tag = tag_bits;
                    cur_slot.valid = true;
                } else {                        // Check if load is a hit
                    load_hits++;
                }
            } else if (v[0].compare("s") == 0) {  
                total_stores++; 
                if (cur_slot.valid == false) {  // Check if store is a miss
                    store_misses++;
                    if (writeAlloc.compare("write-allocate") == 0) { // Check if write-allocate is enabled
                        // Loading from memory 
                        total_loads++; 
                        total_cycles += 100;
                        // Update cache
                        cur_slot.tag = tag_bits;
                        cur_slot.valid = true;
                    } 
                } else {
                    store_hits++;
                    cur_slot.tag = tag_bits; // Writing to cache 
                    if (writeThrough.compare("write-through") == 0) { // Check if write-through is enabled
                        // Writing to memory
                        total_stores++;
                        total_cycles += 100;
                    } else if (writeThrough.compare("write-back") == 0) { // Check if write-back is enabled
                        cur_slot.dirty = true; // Set dirty bit 
                    }
                }
            }
        }
    }
    traceFile.close();
    // Print results
    std::cout << "Total loads: " << total_loads << "\n";
    std::cout << "Total stores: " << total_stores << "\n";
    std::cout << "Load hits: " << load_hits << "\n";
    std::cout << "Load misses: " << load_misses << "\n";
    std::cout << "Store hits: " << store_hits << "\n";  
    std::cout << "Store misses: " << store_misses << "\n";
    std::cout << "Total cycles: " << total_cycles << std::endl;

    return 0;
}