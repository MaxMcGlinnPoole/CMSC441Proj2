#!/bin/bash
#SBATCH --job-name=example
#SBATCH --output slurm_2.out
#SBATCH --error slurm.err
#SBATCH --partition=batch
#SBATCH --qos=short
#SBATCH --constraint=hpcf2013
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1

srun ./driver > 1_run.out
