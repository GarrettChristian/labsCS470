#!/bin/bash
#
#SBATCH --job-name=ep
#SBATCH --output=ep.txt
#SBATCH --nodes=8
#SBATCH --ntasks=64

module load mpi

for n in 1 8 16 32 64; do
    echo "== $n processes =="
    mpirun -np "$n" "ep.C.$n" | grep -E "Time in seconds|Mop/s"
    echo ""
done

