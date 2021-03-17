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