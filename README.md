# Open MPI
This repository contains sample code for running simulations using Message Passing Interface (MPI). It is part of the MPI seminar given at the University of Ottawa.

## Heat
The code heat.cpp is for a simple 1d heat simulation and demonstrates how to set up MPI and how to let the MPI processes communicate and the use of ghost cells.
### Compiling
To compile type

    mpic++ heat.cpp -o heatmpi

## Random Walk
The code randomwalk.cpp is a 3d random walk algorithm showing that distance scales with sqrt(N) where N is the number of steps. It shows the use of MPI_Recuce to gather the results from all of the MPI processes.
### Compiling
To compile type

    mpic++ randomwalk.cpp -o rwalkmpi

