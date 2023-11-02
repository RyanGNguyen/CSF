Experiment Design:

WRITE AND EVICTION POLICIES    

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

gcc.trace data

Parameters | Total Loads | Load Misses | Load Miss Rate | Total Stores | Store Misses | Store Miss Rate | Total Miss Rate | Total Cycles
Allocate & Through & LRU | 318197 | 3399 | 1.068206174 | 197486 | 9236 | 4.676787215 | 2.450148638 | 25318283
Allocate & Through & FIFO | 318197 | 4026 | 1.265253915 | 197486 | 9439 | 4.779579312 | 2.61110023 | 25650283
Allocate & Back & LRU | 318197 | 3399 | 1.068206174 | 197486 | 9236 | 4.676787215 | 2.450148638 | 9344483
Allocate & Back & FIFO | 318197 | 4026 | 1.265253915 | 197486 | 9439 | 4.779579312 | 2.61110023 | 9845283
No Allocate & Through & LRU | 318197 | 6584 | 2.069158414 | 197486 | 32667 | 16.54142572 | 7.611458978 | 22897883
No Allocate & Through & FIFO | 318197 | 7180 | 2.256463763 | 197486 | 33781 | 17.10551634 | 7.943058041 | 23136283

swim.trace data

Parameters | Total Loads | Load Misses | Load Miss Rate | Total Stores | Store Misses | Store Miss Rate | Total Miss Rate	Total Cycles
Allocate & Through & LRU | 220668 | 1161 | 0.5261297515 | 82525	| 10569	| 12.80702817 | 3.868822829 | 13247693
Allocate & Through & FIFO | 220668 | 2311 | 1.047274639 | 82525	| 10738	| 13.0118146 | 4.303859258 | 13775293
Allocate & Back & LRU | 220668 | 1161 | 0.5261297515 | 82525 | 10569 | 12.80702817 | 3.868822829 | 9009593
Allocate & Back & FIFO | 220668 | 2311 | 1.047274639 | 82525 | 10738 | 13.0118146 | 4.303859258	| 9655593
No Allocate & Through & LRU | 220668 | 2596 | 1.176427937 | 82525 | 24495 | 29.68191457 | 8.935232674 | 9594093
No Allocate & Through & FIFO | 220668 | 2700 | 1.223557562 | 82525 | 24531 | 29.72553772 | 8.981407882 | 9635693

After conducting all these trials, there is a clear winner: Write-Allocate, Write-Back, and LRU. For both the 
gcc.trace and swim.trace files, similar trends arose. Both No-Write-Allocate options produced the highest miss 
rates across the board. In the case of gcc.trace, it also produced very high cycle counts. This immediately ruled
out these options. 

Among the Write-Allocate options, performance was close. For store miss rates and total miss rates, all the options 
performed similarly. However, when it came to the load miss rates, the LRU options showed modest improvement with the
gcc.trace file but a large improvement with the swim.trace file. Finally, the tie breaker came when accounting for
Total Cycles. Write-Through displayed some of the highest total cycles of all the options while Write-Back had the
least. All things considered, Write-Allocate, Write-Back, and LRU outperformed in almost every metric. 

This intuitively makes sense. Write-allocate is good in the case where additional writes to the location will follow. 
Write-Back is also good since it avoids having to make unnecessary time-consuming writes to memory. Lastly, LRU also
makes sense since it keeps the information most used by the program and discards the least used first. 

CACHE TYPE

Now that we have figured out the optimal write and eviction policies, we will test which cache configuration is 
optimal. In theory, set-associative caches should be the best since they reduce the worst aspects of both direct 
mapping (many blocks contending for the same slot -> many cache misses) and associative mapping (finding blocks
in cache is expensive -> very slow, power-hungry performance). To test this, three different cache set-ups will 
be used. The first is the set up from the first half of the experiment (set-associative: 256, 4, 16). The next 
cache will be direct-mapped (1024, 1, 16), and the last will be associative (1, 1024, 16). Notice how each cache
is the same size for consistency. They will be run with Write-Through, Write-Back, and LRU policies on the gcc.trace
and swim.trace files using the same metrics.

gcc.trace data

Parameters | Total Loads | Load Misses | Load Miss Rate | Total Stores | Store Misses | Store Miss Rate | Total Miss Rate | Total Cycles
set-associative | 318197 | 3399 | 1.068206174 | 197486 | 9236 | 4.676787215 | 2.450148638 | 25318283
direct-mapping | 318197 | 5959 | 1.872739215 | 197486 | 9984 | 5.055548241 | 3.091627996 | 11127283
associative | 318197 | 3224 | 1.013208798 | 197486 | 9186 | 4.651468965 | 2.406517182 | 39226883

swim.trace data

Parameters | Total Loads | Load Misses | Load Miss Rate | Total Stores | Store Misses | Store Miss Rate | Total Miss Rate | Total Cycles
set-associative | 220668 | 1161	| 0.5261297515 | 82525 | 10569 | 12.80702817 | 3.868822829 | 13247693
direct-mapping | 220668 | 2933 | 1.329146047 | 82525 | 10987 | 13.31354135 | 4.59113502 | 10171593
associative | 220668 | 1098 | 0.497580075 | 82525 | 10556 | 12.79127537 | 3.843756287 | 18965993

The data follows our initial hypothesis exactly. Direct-mapping experienced the fastest performance (lowest total 
cycles) but also the highest miss rates across every category. Associative caching had similar miss rates as 
set-associative, but it suffered the worst performance. Set-associative mapping provided the best of both caches:
fast performance and minimal misses. 

Collaborators:
    Ryan Nguyen (rnguye14)
     - Main
     - Error checking
     - Cache initialization
     - Cache tag and index
     - print statistics 
     - Experiment

    Andy Tang (atang24)
     - 
