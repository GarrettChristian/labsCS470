# Introduction

These are compiled NAS Parallel Benchmark executables. Some are compiled with
OpenMP (`omp` folder) and parallelize across multiple threads on a single node,
while some are compiled with MPI (`mpi` folder) and parallelize across multiple
processes on multiple nodes.

The name of the executables are composed of the name of the benchmark (e.g.,
`ep` or `lu`), the problem size (`A` is smaller than `C`), and the number of MPI
processes (or `x` in the case of an OpenMP version).

# Interactive Submission

This section shows how to invoke SLURM utilities to run the benchmarks on
compute nodes using an interactive job.

## OpenMP

Use the `srun` command to run a non-MPI job interactively:

    OMP_NUM_THREADS=1 srun ep.A.x
    OMP_NUM_THREADS=2 srun ep.A.x
    OMP_NUM_THREADS=3 srun ep.A.x
    OMP_NUM_THREADS=4 srun ep.A.x

## MPI

Use the `salloc` command to run an MPI job interactively:

    module load mpi
    salloc -n 8 mpirun ep.C.8
    salloc -n 16 mpirun ep.C.16
    salloc -n 24 mpirun ep.C.24
    salloc -n 32 mpirun ep.C.32

# Batch Submission

The `run.sh` scripts show how to run OpenMP and MPI programs for multiple
thread/process counts within a single job. To submit one of these scripts as a
batch job, use the `sbatch` command:

    sbatch run.sh

