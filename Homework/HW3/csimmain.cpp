#include "csimfuncs.h"

int main(int argc, char* argv[]) {
    // Check if arguments are valid
    checkArgs(argc, argv); 

    // Initialize cache
    Cache cache(argv); 
    
    std::string line;
    while (std::getline(std::cin, line)){
        std::istringstream iss(line);               // Convert each line into a stringstream
        std::vector<std::string> v; 
        std::string word;   
        for (unsigned i = 0; i < 3; i++) {       // Divide each line into 3 words
            iss >> word; 
            v.push_back(word);                   // Store each word in a vector 
        }
        std::uint32_t address = std::stoul(v[1], NULL, 16);  // Convert hex address to unsigned int
        unsigned byteBits = log2(cache.numBytes);                    // Calculate number of bits for byte offset
        unsigned setBits = log2(cache.numSets);                      // Calculate number of bits for set index
        if (v[0].compare("s") == 0) { 
            cache.store(address, byteBits, setBits);                          // Store address
        } else {
            cache.load(address, byteBits, setBits);                         // Load address
        }
    }
    cache.print_statistics();                   // Print statistics

    return 0;
}