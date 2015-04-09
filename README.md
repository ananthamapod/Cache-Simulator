# Cache-Simulator
Cache Simulator in C written for Computer Architecture course CS 198:211 at Rutgers University

This is a simple cache simulator that calculates the number of memory writes and memory reads for a memory access pattern provided as a file.
The inputs to the program, aside from the file listing the memory accesses, are parameters that specify the cache to be simulated.

Note: Although the help function suggests n-associative caches are supported, there is not yet any provision for modelling n-associative caches. Only direct association is currently supported.
Moreover, the only replacement strategy currently implemented is the First-In-First-Out (FIFO) strategy, although the intention is to support Least-Recently-Used (LRU) replacement as well in the future.
