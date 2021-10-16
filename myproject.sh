!/bin/bash

File: myproject.sh

SBATCH --job-name=myproject  # a logical name of your choice
SBATCH --ntasks=30           # like doing -n 30
SBATCH --mem=4gb             # Each task gets this much memory (if available)
SBATCH --time=00:15:00       # Time limit in the form hh:mm:ss
SBATCH --output=out/%j.log   # any stdout will be redirected to the file out/[JOB ID].log

module load mpi/mpich-3.2-x86_64

# For a python3 project
# mpirun python3 ~/Projects/myproject/myproject.py

# For a C MPI project
# mpirun ~/Projects/myproject/a.out 