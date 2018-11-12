#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int a[80000] = {0};
  int b[80000];

  // This causes a deadlock because process 0 is sending to 1 and
  // process 1 is sending to 0. Both processes are waiting for the
  // sending to succeed before receiving.
  if(rank == 0) {
    MPI_Send(a, 80000, MPI_INT, 1, 0, MPI_COMM_WORLD);
    MPI_Recv(b, 80000, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  else {
    MPI_Send(a, 80000, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Recv(b, 80000, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  // We will never get here
  MPI_Finalize();
}
