#!/bin/bash
#
#SBATCH --job-name=ep
#SBATCH --output=ep.txt
#SBATCH --ntasks=1

for i in 1 2 4 8; do
    echo "== $i thread(s) =="
    export OMP_NUM_THREADS="$i"
    ./ep.A.x | grep -E "Time in seconds|Mop/s"
    echo ""
done

