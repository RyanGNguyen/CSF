#include "csimfuncs.h"

Cache::Cache(char* parameters[]) {
    numSets = std::stoul(parameters[1]);
    numBlocks = std::stoul(parameters[2]);
    numBytes = std::stoul(parameters[3]);
    allocate_or_no = strcmp(parameters[4], "write-allocate") == 0 ? true : false;
    through_or_back = strcmp(parameters[5], "write-through") == 0 ? true : false;
    lru_fifo = strcmp(parameters[6], "lru") == 0 ? true : false;

    for (int i = 0; i < numSets; i++) {
        Set set;
        sets.push_back(set);
    }
}

unsigned int Cache::get_tag(std::uint32_t address) {
    return address >> (((unsigned) log2(numBytes)) + ((unsigned) log2(numSets)));
}

unsigned int Cache::get_index(std::uint32_t address) {
    return (address >> ((unsigned) log2(numBytes))) & (numSets - 1);
}

bool Cache::find_hit(unsigned int index, unsigned int tag) {
    if (sets[index].slot_tag_map.find(tag) != sets[index].slot_tag_map.end()) {
        return true;
    }
    return false;
}

void Cache::runTrace(std::string instruction, std::uint32_t address)  {
    if (instruction.compare("l") == 0) {
        load(address);
    } else {
        store(address);
    }
}

void Cache::load(std::uint32_t address) {
    unsigned int index = get_index(address);
    unsigned int tag = get_tag(address);

    total_loads++;
    total_cycles++;

    // check for load hit/miss
    if (find_hit(index, tag)) {
        load_hits++;

        if (lru_fifo) { // move the least recently accessed slot (first slot of the block) to the end since it was just accessed
            Set &s = sets[index];
            Slot slot = *s.slot_tag_map[tag];
            s.slots.erase(s.slot_tag_map[tag]);
            s.slots.push_back(slot);
            s.slot_tag_map[tag] = --s.slots.end();
        }

    } else { // load was a miss
        load_misses++;
        add_slot(index, tag); // if load misses, add the slot to the cache
    }
}

void Cache::store(std::uint32_t address) {
    unsigned index = get_index(address);
    unsigned tag = get_tag(address);

    total_stores++;
    total_cycles++;

    if (!allocate_or_no) { // store for no-write-allocate does not modify the cache
        
        // check for store hit/miss
        if (find_hit(index, tag)) {

            // if is lru
            if (lru_fifo) {
                Set &s = sets[index];
                Slot slot = *s.slot_tag_map[tag];
                s.slots.erase(s.slot_tag_map[tag]);
                s.slots.push_back(slot);
                s.slot_tag_map[tag] = --s.slots.end();
            }

            store_hits++;

        } else { // store is a miss
            store_misses++;
        }

    } else { // store for write-allocate, block is brought into cache before the store

        // check for store hit/miss
        if (find_hit(index, tag)) {

            // if is lru
            if (lru_fifo) {
                Set &s = sets[index];
                Slot slot = *s.slot_tag_map[tag];
                s.slots.erase(s.slot_tag_map[tag]);
                s.slots.push_back(slot);
                s.slot_tag_map[tag] = --s.slots.end();
            }

            store_hits++;

        } else { // store is a miss
            store_misses++;
            add_slot(index, tag);
        }

        if (!through_or_back) { // if is write-back, block is marked dirty
            Set &s = sets[index];
            s.slot_tag_map[tag]->dirty = true;
        }
    }
    if (through_or_back) { // if is write-through, store writes to both the cache and memory
        total_cycles += 100; 
    }
}

void Cache::add_slot(unsigned int index, unsigned int tag) {
    Set &s = sets[index];

    // check if the slot is full
    if (s.slots.size() == (size_t) numBlocks) { // slot is full
        s.slot_tag_map.erase(s.slot_tag_map.find(s.slots.front().tag)); // remove the evicted slot from the map
        if (s.slots.front().dirty) {
            total_cycles += (numBytes/4) * 100;
        }
        s.slots.pop_front(); // evict the least recently used slot from the block
    }

    Slot slot;
    slot.tag = tag;

    s.slots.push_back(slot);
    s.slot_tag_map[tag] = --s.slots.end();
    total_cycles += (numBytes/4) * 100;
}

void Cache::print_statistics() {
    std::cout << "Total loads: " << total_loads << "\n";
    std::cout << "Total stores: " << total_stores << "\n";
    std::cout << "Load hits: " << load_hits << "\n";
    std::cout << "Load misses: " << load_misses << "\n";
    std::cout << "Store hits: " << store_hits << "\n";  
    std::cout << "Store misses: " << store_misses << "\n";
    std::cout << "Total cycles: " << total_cycles << std::endl;
}

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

void checkArgc(unsigned int argc) {
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