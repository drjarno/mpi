#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int a, b;
  double c;
  if(rank == 0) {
    a = 1;
    b = 40000;
    c = 3.4;

    std::cout << "Sending a" << std::endl;
    MPI_Send(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    std::cout << "Sending b" << std::endl;
    MPI_Send(&b, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    std::cout << "Sending c" << std::endl;
    MPI_Send(&c, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
    std::cout << "Done sending" << std::endl;
  }
  else if(rank == 1) {
    MPI_Recv(&a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << "Received a (" << a << ")" << std::endl;
    MPI_Recv(&c, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << "Received c (" << c << ")" << std::endl;
    MPI_Recv(&b, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << "Received b (" << b << ")" << std::endl;
    std::cout << "Done receiving" << std::endl;
  }

  MPI_Finalize();
}
