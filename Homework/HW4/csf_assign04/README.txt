CONTRIBUTIONS

Ryan Nguyen:
main, merge, merge_sort parallelization, error handling

Andy Tang:
main, merge, merge_sort parallelization, error handling

REPORT

Experiment Results:

Test run with threshold 2097152

real    0m0.398s
user    0m0.365s
sys     0m0.015s
Test run with threshold 1048576

real    0m0.244s
user    0m0.389s
sys     0m0.033s
Test run with threshold 524288

real    0m0.179s
user    0m0.427s
sys     0m0.038s
Test run with threshold 262144

real    0m0.144s
user    0m0.538s
sys     0m0.053s
Test run with threshold 131072

real    0m0.145s
user    0m0.539s
sys     0m0.060s
Test run with threshold 65536

real    0m0.158s
user    0m0.569s
sys     0m0.081s
Test run with threshold 32768

real    0m0.161s
user    0m0.561s
sys     0m0.142s
Test run with threshold 16384

real    0m0.169s
user    0m0.597s
sys     0m0.171s

Discussion:

With a high threshold, there are fewer degrees of parallelization which means more instances of sequential sorting
which is slow. This is reflected in the longer times of the higher threshold value experiments. At lower thresholds,
there are higher degrees of parallelization which means fewer instances of sequential sorting. This is reflected in 
the shorter times of the lower threshold value experiments. This time decrease is not constant, however, since at 
lower thresholds the time performance of sequential and merge sorting converge at the smaller partitions, thus 
the increase in performance is less and less pronounced between the lower thresholds.