# OpenMP Workshop

## Overview

-   first standard in 1997
-   initially: thread parallelism, focus on loops
-   OpenMP 3.0: Tasks
-   OpenMP 4.0: SIMD parallelism, offloading to GPGPUs
-   OpenMP 5.0: Tools and Debugging Interfaces
-   compiler directives (`#pragma omp <…>`)
-   library interface (`omp_<function>(arguments)`)

## Basics

### Parallel Regions

-   `#pragma omp parallel`
-   code in such a region is executed by a number of threads
-   programmer has to ensure that the code can be run in parallel
-   number of threads can be
    -   default (usually the number of available CPUs)
    -   set in an environment variable (`OMP_NUM_THREADS`)
    -   set outside of a parallel region with `omp_set_num_threads()`

### Parallel For Loop

-   `#pragma omp parallel for`
-   can be placed in front of a for loop
-   will distribute the iterations of the loop to different threads
-   programmer has to ensure that there are no data dependencies between iterations

#### Loop Partitioning

-   `#pragma omp parallel for schedule(kind, chunk_size)`
-   chunk_size: number of iterations per chunk
-   kind
    -   static: threads get their work in a round robin fashion, [image](static_scheduling.png)
    -   dynamic: threads pick up work on a first come first serve basis
    -   guided: [image](guided_scheduling.png)
    -   auto: compiler and/or runtime system decides
    -   runtime: schedule and chunk size are decided at runtime by Internal Control Variables

### Parallel Sections

-   `#pragma omp parallel sections`
-   each `section` inside of `sections` is executed in parallel

### Locks

-   changes the way execution is parallelized for parts of the code in the parallel region
-   `single`: code is executed on a single thread only, instead of all of the threads in the parallel region
-   `master`: code is executed on the master thread only
-   `critical`: code inside this scope is executed sequentially

### Synchronization

-   these constructs synchronize between multiple threads of execution
-   `barrier`: only continues execution after all threads have reached that point
-   `critical`: ensures only one thread executes the block of code at a time
-   `reduction`: allows efficient parallel reduction operations, like sum, difference, min or max

## Practical Examples

### Things to look out for

-   hardest part is getting the algorithm and data into a form that can be parallelized
    -   thinking in independent loop iterations helps with this
-   Windows compiler does not support newest version of OpenMP
    -   can only parallelize loops that have the form `for (int i = 0; i < …; i++) {`
    -   does not support reduction

### Image Processing

-   simple `*.bmp` reader
-   large image files can be processed much faster in parallel

### Marching Cubes

#### The Algorithm

-   given is a function f(x,y,z)
-   sample f at regular intervals
-   if sample is below threshold
    -   we are inside the volume
-   else
    -   we are outside the volume
-   construct triangle mesh that approximates the surface
    -   take 8 samples (corners of a cube)
    -   depending on which corners are inside of the surface, append different triangles to total list of triangles

#### Parallelization

-   not as trivial as we would like
-   parallelize execution over one dimension (z in this case)
-   construct temporary list of vertices
-   run algorithm and insert vertices into temporary list
-   copy list of vertices over to the final list sequentially

### DTM Viewer

-   DTM == Digital Terrain Model
-   "Staatlicher Geologischer Dienst Sachsen" provides DTM data for Saxony
-   DTM is about 1.3GB in size
-   loading and processing can be accelerated with openmp
-   loading
    -   need to load almost 5000 files into memory and parse 3D points from them
    -   process all files in parallel and append work units to a queue
-   processing
    -   pop one work unit from queue at a time and create an OpenMP task to process it
    -   processing involves creating a triangle mesh from the loaded points and uploading it to the GPU

Results:

|            | sequentially       | parallel           |
| ---------- | ------------------ | ------------------ |
| loading    | 270 files/s or 25s | 600 files/s or 10s |
| processing | 20 files/s         | 55 files/s         |
