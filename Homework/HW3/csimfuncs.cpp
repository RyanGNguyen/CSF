#include "csimfuncs.h"

void checkArgs(int argc, char* argv[]) {
    checkArgc(argc);
    checkNumSets(argv[1]);                     
    checkNumBlocks(argv[2]);
    checkNumBytes(argv[3]);
    checkWriteAlloc(argv[4]);
    checkWriteThrough(argv[5]);
    checkWriteBackAndNoAlloc(argv[4], argv[5]);
    checkEviction(argv[2], argv[6]); 
}

void checkArgc(unsigned argc) {
    if (argc != 7) {
        std::cerr << "Incorrect number of arguments" << std::endl;
        exit(1); 
    }  
}

void checkNumSets(std::string arg1) {
    // Check if number of sets is a valid number
    unsigned numSets = 0;
    try {
        numSets = std::stoul(arg1, NULL, 10);      
    } catch (const std::invalid_argument &e) {
        std::cerr << "Invalid argument for number of sets" << std::endl;
        exit(2);
    } catch (const std::out_of_range &e) {
        std::cerr << "Number of sets out of range" << std::endl;
        exit(2); 
    }
    // Check if number of sets is 0
    if (numSets == 0) {
        std::cerr << "Number of sets cannot be 0" << std::endl;
        exit(2); 
    }
    // Check if number of sets is a power of 2
    unsigned powSets = numSets & (numSets - 1);
    if (powSets != 0) {
        std::cerr << "Number of sets is not a power of 2" << std::endl;
        exit(2); 
    }
}

void checkNumBlocks(std::string arg2) {
    // Check if number of blocks is a valid number
    unsigned numBlocks = 0;
    try {
        numBlocks = std::stoul(arg2, NULL, 10);      
    } catch (const std::invalid_argument &e) {
        std::cerr << "Invalid argument for number of blocks" << std::endl;
        exit(3); 
    } catch (const std::out_of_range &e) {
        std::cerr << "Number of blocks out of range" << std::endl;
        exit(3); 
    }
    // Check if number of blocks is 0
    if (numBlocks == 0) {
        std::cerr << "Number of blocks cannot be 0" << std::endl;
        exit(3); 
    }
    // Check if number of blocks is a power of 2
    unsigned powBlocks = numBlocks & (numBlocks - 1);
    if (powBlocks != 0) {
        std::cerr << "Number of blocks is not a power of 2" << std::endl;
        exit(3); 
    }
}

void checkNumBytes(std::string arg3) {
    // Check if number of bytes per block is a valid number
    unsigned numBytes = 0;
    try {
        numBytes = std::stoul(arg3, NULL, 10);      
    } catch (const std::invalid_argument &e) {
        std::cerr << "Invalid argument for number of bytes" << std::endl;
        exit(4); 
    } catch (const std::out_of_range &e) {
        std::cerr << "Number of bytes out of range" << std::endl;
        exit(4); 
    }
    // Check if number of bytes is less than 4
    if (numBytes < 4) {
        std::cerr << "Number of bytes cannot be less than 4" << std::endl;
        exit(4); 
    }
    // Check if number of bytes is a power of 2
    unsigned powBytes = numBytes & (numBytes - 1);
    if (powBytes != 0) {
        std::cerr << "Number of bytes is not a power of 2" << std::endl;
        exit(4); 
    }
}

void checkWriteAlloc(std::string arg4) {
    // Check if write-allocate is a valid argument
    if (arg4.compare("write-allocate") != 0 && arg4.compare("no-write-allocate") != 0) {
        std::cerr << "Invalid argument for write-allocate" << std::endl;
        exit(5); 
    }
}

void checkWriteThrough(std::string arg5) {
    // Check if write-through is a valid argument
    if (arg5.compare("write-through") != 0 && arg5.compare("write-back") != 0) {
        std::cerr << "Invalid argument for write-through" << std::endl;
        exit(6); 
    }
}

void checkWriteBackAndNoAlloc(std::string arg4, std::string arg5) {
    // Check if write-back and no-write-allocate are used together
    if (arg4.compare("no-write-allocate") == 0 && arg5.compare("write-back") == 0) {
        std::cerr << "Cannot use no-write-allocate and write-back together" << std::endl;
        exit(7); 
    }
}

void checkEviction(std::string arg2, std::string arg6) {
    // Check if eviction policy is called for a direct-mapped cache
    unsigned numBlocks = std::stoul(arg2, NULL, 10);
    if (numBlocks == 1) {
        std::cerr << "Eviction policies are invalid for a direct-mapped cache" << std::endl;
        exit(8); 
    } else {
        // Check if eviction policy is valid
        if (arg6.compare("lru") != 0 && arg6.compare("fifo") != 0) {
            std::cerr << "Invalid argument for eviction policy" << std::endl;
            exit(8); 
        }
    }
}

std::uint32_t get_tag(std::uint32_t address, int setBits, int blockBits) {
    return address >> setBits + blockBits;
}

std::uint32_t get_set(std::uint32_t address, int numSets, int blockBits) {
    int set_mask = numSets- 1;
    return (address >> blockBits) & set_mask;
}
