# OpenMP Workshop

## Overview

- first standard in 1997
- initially: thread parallelism, focus on loops
- OpenMP 3.0: Tasks
- OpenMP 4.0: SIMD parallelism, offloading to GPGPUs
- OpenMP 5.0: Tools and Debugging Interfaces
- compiler directives (`#pragma omp <…>`)
- library interface (`omp_<function>(arguments)`)

## Basics

### Parallel Regions

- `#pragma omp parallel`
- code in such a region is executed by a number of threads
- programmer has to ensure that the code can be run in parallel
- number of threads can be
    - default (usually the number of available CPUs)
    - set in an environment variable (`OMP_NUM_THREADS`)
    - set outside of a parallel region with `omp_set_num_threads()`

### Parallel For Loop

- `#pragma omp parallel for`
- can be placed in front of a for loop
- will distribute the iterations of the loop to different threads
- programmer has to ensure that there are no data dependencies between iterations

#### Loop Partitioning

- `#pragma omp parallel for schedule(kind, chunk_size)`
- chunk_size: number of iterations per chunk
- kind
    - static: threads get their work in a round robin fashion, [image](static_scheduling.png)
    - dynamic: threads pick up work on a first come first serve basis
    - guided: [image](guided_scheduling.png)
    - auto: compiler and/or runtime system decides
    - runtime: schedule and chunk size are decided at runtime by Internal Control Variables

### Parallel Sections

- `#pragma omp parallel sections`
- each `section` inside of `sections` is executed in parallel

### Locks

- changes the way execution is parallelized for parts of the code in the parallel region
- `single`: code is executed on a single thread only, instead of all of the threads in the parallel region
- `master`: code is executed on the master thread only
- `critical`: code inside this scope is executed sequentially

### Synchronization

- these constructs synchronize between multiple threads of execution
- `barrier`: only continues execution after all threads have reached that point
- `critical`: ensures only one thread executes the block of code at a time
- `reduction`: allows efficient parallel reduction operations, like sum, difference, min or max



## Practical Examples

### Image Processing

- simple `*.bmp` reader
- large image files can be processed much faster in parallel

### Marching Cubes

- TODO add explanation of algorithm
- TODO add explanation on how to parallelize it with omp
