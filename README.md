[français](#markdown-header-seminaire-mpi) | [english](#markdown-header-mpi-seminar)
# Séminaire MPI
Ce dépôt du code source contient des exemples de code pour exécuter des simulations à l'aide de « Message Parsing Interface (MPI) ». Il fait partie du séminaire MPI donné à l'Université d'Ottawa.

Vous pourriez exécuter les exemples avec la commande:

    mpirun -np X ./programme

Remplacez « X » avec le nombre de processus que vous désirez utiliser et « programme » avec le nom de l'exemple que vous souhaitez exécuter.
## Chaleur
Le code source « heat.cpp » est pour une simple simulation de chaleur en une dimension et cela montre comment les processus MPI se communiquent et l'usage de cellules fantômes.
### Compiler
Pour compiler, tapez

    mpic++ heat.cpp -o heatmpi

### Tracer
Si vous avez ffmpeg et gnuplot installés sous Linux ou macOS, vous pouvez utiliser plot.sh pour créer un film des résultats de la simulation.
## Marche aléatoire
Le code source « randomwalk.cpp » est un algorithme de marche aléatoire démontrant que la distance varie proportionnellement à l'inverse de racine carré du nombre de pas. Cela montre l'usage de « MPI_Reduce » pour rassembler les résultats de tous les processus MPI.
### Compiler
Pour compiler, tapez

    mpic++ randomwalk.cpp -o randomwalk

## Impasse
Le code source « deadlock.cpp » démontre une erreur fréquente où un processus MPI attend un autre alors que l'autre attend le premier. Alors ils attendent indéfinement.
### Compiler
Pour compiler, tapez

    mpic++ deadlock.cpp -o deadlock

Essayez d'échanger la ligne 24 avec 25 pour corriger l'impasse

## Disparité
L'ordre dans lequel les choses sont envoyées et reçues est important. Le code source « mismatch.cpp » montre qui se passe si l'ordre est mauvais.
### Compiler
Pour compiler, tapez

    mpic++ mismatch.cpp -o mismatch


# MPI Seminar
This repository contains sample code for running simulations using Message Passing Interface (MPI). It is part of the MPI seminar given at the University of Ottawa.

The examples can be run with

    mpirun -np X ./program

Replace "X" with the number of processes you want to use and "program" by the name of the example program you wish to run.
## Heat
The code heat.cpp is for a simple 1d heat simulation and demonstrates how to set up MPI and how to let the MPI processes communicate and the use of ghost cells.
### Compiling
To compile type

    mpic++ heat.cpp -o heatmpi

### Plotting
If you have ffmpeg and gnuplot installed on Linux or macOS, you can use plot.sh to create a movie of the simulation results.
## Random Walk
The code randomwalk.cpp is a 3d random walk algorithm showing that distance scales with sqrt(N) where N is the number of steps. It shows the use of MPI_Recuce to gather the results from all of the MPI processes.
### Compiling
To compile type

    mpic++ randomwalk.cpp -o rwalkmpi

## Deadlock
The code deadlock.cpp shows a common mistake where one MPI process waits for the other to complete while the other waits for the first. Thus they will wait for each other indefinitely.
### Compiling
To compile type

    mpic++ deadlock.cpp -o deadlock

Try switching lines 24 and 25 to fix the deadlock

## Mismatch
The order in which things are sent and received is important. The code mismatch.cpp shows what happens when the order is wrong.
### Compiling
To compile type

    mpic++ mismatch.cpp -o mismatch
