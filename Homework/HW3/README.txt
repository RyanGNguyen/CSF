Experiment Design:

The cache simulation was ran on the available trace files taken from real program data (gcc.trace & swim.trace). 
This is because the read and write trace files provided are too basic to provide any considerable insights into 
the cache configurations. Each time, the size of the cache was held constant at 256 sets of 4 blocks each (aka a 
4-way set-associative cache), with each block containing 16 bytes of memory. This set up seemed like a good middle
ground between pure direct-mapped and pure associative caches with a moderate size. Write-Allocate & Write-Through, 
Write-Allocate & Write-Back, and No-Write-Allocate & Write-Through were tested with both LRU and FIFO eviction 
policies. For the sake of time, each configuration was only ran once per trace file.

Four main factors were taken into account: Load Miss Rate, Store Miss Rate, Total Miss Rate, and Total Cycles. The 
first two metrics were calculated from the cache output and converted into a percent (e.g. Load Miss Rate = Load 
Misses / Total Loads * 100). Similarly, Total Miss Rate was calculated through dividing the sum of load and store 
misses by the sum of total loads and total stores and converting to a percent. Total Cycles was directly provided. 
In theory, the optimal configuration would minimize all four metrics. 

After conducting all these trials, there is a clear winner: Write-Allocate, Write-Back, and LRU. For both the 
gcc.trace and swim.trace files, similar trends arose. Both No-Write-Allocate options produced the highest miss 
rates across the board. In the case of gcc.trace, it also produced very high cycle counts. This immediately ruled
out these options. 

Among the Write-Allocate options, performance was close. For store miss rates and total miss rates, all the options 
performed similarly. However, when it came to the load miss rates, the LRU options showed modest improvement with the
gcc.trace file but a large improvement with the swim.trace file. Finally, the tie breaker came when accounting for
Total Cycles. Write-Through displayed some of the highest total cycles of all the options while Write-Back had the
least. All things considered, Write-Allocate, Write-Back, and LRU outperformed in almost every metric. 

Collaborators:
    Ryan Nguyen (rnguye14)
     - 
    Andy Tang (atang24)
     - 
