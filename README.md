# Network-on-Chip-mapping

### Implementation of the Differential Evolution algorithm for efficient core-router mapping in a mesh topology Network-on-Chip.

More information on Differential Evolution algorithm can be found [here](https://en.wikipedia.org/wiki/Differential_evolution)

The problem is combinatorial in nature and hence the algorithm has been redefined for that purpose.
- The mappings of cores to routers are taken as vectors
- The difference between population members is quantified through dissimilarity in mappings
- The mutation and crossover operations are performed through core swaps between router positions

Fabric size, core count, algorithm parameters can be edited from the declarations in main function.

Files included:

:eyes: Bandwidth files

:eyes: C++ source code

__Compilation__ : *g++ <source_code.cpp> -o <out_file.exe>*
