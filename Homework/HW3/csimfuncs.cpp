#include "csimfuncs.h"

int checkArgs(unsigned argc, char* argv[]) {
    int sumErr = 0;                                  // Subject to change
    sumErr += checkArgc(argc);
    sumErr += checkNumSets(argv[1]);                     
    sumErr += checkNumBlocks(argv[2]);
    sumErr += checkNumBytes(argv[3]);
    sumErr += checkWriteAlloc(argv[4]);
    sumErr += checkWriteThrough(argv[5]);
    sumErr += checkWriteBackAndNoAlloc(argv[4], argv[5]);

    return sumErr;
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

int checkEviction(std::string arg1, std::string arg2, std::string arg6) {
    //TODO
}