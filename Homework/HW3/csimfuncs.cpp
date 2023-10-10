#include "csimfuncs.h"

int checkArgs(unsigned argc, char* argv[]) {
    int err = 0;                                  // Subject to change
    err += checkArgc(argc);
    err += checkNumSets(argv[1]);                     
    err += checkNumBlocks(argv[2]);
    err += checkNumBytes(argv[3]);
    err += checkWriteAlloc(argv[4]);
    err += checkWriteThrough(argv[5]);
    err += checkWriteBackAndNoAlloc(argv[4], argv[5]);
    err += checkEviction(argc, argv[1], argv[2], argv[6]); 
    err += checkArg7(argc, argv[7]);
    err += checkTraceFile(argv[8]);
    return err;
}

int checkArgc(unsigned argc) {
    if (argc < 8 || argc > 9) {
        std::cerr << "Incorrect number of arguments" << std::endl;
        return 1; 
    } 
    return 0; 
}

int checkNumSets(std::string arg1) {
    // Check if number of sets is a valid number
    unsigned numSets = 0;
    try {
        numSets = std::stoul(arg1, NULL, 10);      
    } catch (const std::invalid_argument &e) {
        std::cerr << "Invalid argument for number of sets" << std::endl;
        return 1;
    } catch (const std::out_of_range &e) {
        std::cerr << "Number of sets out of range" << std::endl;
        return 1;
    }
    // Check if number of sets is 0
    if (numSets == 0) {
        std::cerr << "Number of sets cannot be 0" << std::endl;
        return 1;
    }
    // Check if number of sets is a power of 2
    unsigned powSets = numSets & (numSets - 1);
    if (powSets != 0) {
        std::cerr << "Number of sets is not a power of 2" << std::endl;
        return 1;
    }
    return 0; 
}

int checkNumBlocks(std::string arg2) {
    // Check if number of blocks is a valid number
    unsigned numBlocks = 0;
    try {
        numBlocks = std::stoul(arg2, NULL, 10);      
    } catch (const std::invalid_argument &e) {
        std::cerr << "Invalid argument for number of blocks" << std::endl;
        return 1;
    } catch (const std::out_of_range &e) {
        std::cerr << "Number of blocks out of range" << std::endl;
        return 1;
    }
    // Check if number of blocks is 0
    if (numBlocks == 0) {
        std::cerr << "Number of blocks cannot be 0" << std::endl;
        return 1;
    }
    // Check if number of blocks is a power of 2
    unsigned powBlocks = numBlocks & (numBlocks - 1);
    if (powBlocks != 0) {
        std::cerr << "Number of blocks is not a power of 2" << std::endl;
        return 1;
    }
    return 0;
}

int checkNumBytes(std::string arg3) {
    // Check if number of bytes per block is a valid number
    unsigned numBytes = 0;
    try {
        numBytes = std::stoul(arg3, NULL, 10);      
    } catch (const std::invalid_argument &e) {
        std::cerr << "Invalid argument for number of bytes" << std::endl;
        return 1;
    } catch (const std::out_of_range &e) {
        std::cerr << "Number of bytes out of range" << std::endl;
        return 1;
    }
    // Check if number of bytes is less than 4
    if (numBytes < 4) {
        std::cerr << "Number of bytes cannot be less than 4" << std::endl;
        return 1;
    }
    // Check if number of bytes is a power of 2
    unsigned powBytes = numBytes & (numBytes - 1);
    if (powBytes != 0) {
        std::cerr << "Number of bytes is not a power of 2" << std::endl;
        return 1;
    }
    return 0; 
}

int checkWriteAlloc(std::string arg4) {
    // Check if write-allocate is a valid argument
    if (arg4.compare("write-allocate") != 0 && arg4.compare("no-write-allocate") != 0) {
        std::cerr << "Invalid argument for write-allocate" << std::endl;
        return 1;
    }
    return 0;
}

int checkWriteThrough(std::string arg5) {
    // Check if write-through is a valid argument
    if (arg5.compare("write-through") != 0 && arg5.compare("write-back") != 0) {
        std::cerr << "Invalid argument for write-through" << std::endl;
        return 1;
    }
    return 0;
}

int checkWriteBackAndNoAlloc(std::string arg4, std::string arg5) {
    // Check if write-back and no-write-allocate are used together
    if (arg4.compare("no-write-allocate") == 0 && arg5.compare("write-back") == 0) {
        std::cerr << "Cannot use no-write-allocate and write-back together" << std::endl;
        return 1;
    }
    return 0;
}

int checkEviction(unsigned argc, std::string arg1, std::string arg2, std::string arg6) {
    // If argc == 8, no eviction policy was passed as parameter
    if (argc == 8) {
        return 0;
    }
    // Check if eviction policy is called for a direct-mapped cache
    unsigned numSets = std::stoul(arg1, NULL, 10);
    unsigned numBlocks = std::stoul(arg2, NULL, 10);
    if (numSets > 1 && numBlocks == 1) {
        std::cerr << "Eviction policies are invalid for a direct-mapped cache" << std::endl;
        return 1;
    } else {
        // Check if eviction policy is valid
        if (arg6.compare("lru") != 0 && arg6.compare("fifo") != 0) {
            std::cerr << "Invalid argument for eviction policy" << std::endl;
            return 1;
        }
    }
    return 0; 
}

int checkArg7(unsigned argc, std::string arg7) {
    unsigned index = (argc == 8) ? 6 : 7;
    // Check if argument 7 is less than sign
    if (arg7.compare("<") != 0) {
        std::cerr << "Invalid argument for argument" << index << std::endl;
        return 1;
    }
    return 0; 
}

int checkTraceFile(std::string arg8) {
    // Check if trace file is valid
    std::ifstream tracefile(arg8);
    if (!tracefile.is_open()) {
        std::cerr << "Invalid trace file" << std::endl;
        return 1;
    }
    return 0;
}