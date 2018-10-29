#include <iostream>
#include <fstream>
#include <mpi.h>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int num_processes;
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double starttime = 0;
  // Only use process 0 to output text
  if(rank == 0) {
    std::cout << "Using " << num_processes << " processes" << std::endl;
    starttime = MPI_Wtime();
  }

  const int gridsize = 400;
  const int num_iterations = 50000;
  const int save_every = 100;
  const double alpha = 0.1;
  const double dt = 0.05;
  const double dx = 0.1;

  // Divide the grid over all processes and add ghost cell
  int localgridsize = gridsize / num_processes + 2;
  if(rank == num_processes-1)
    localgridsize += gridsize % num_processes;

  double *localgrid_now = new double[localgridsize];
  double *localgrid_future = new double[localgridsize];

  // Init
  for(int i = 0; i < localgridsize; i++)
    localgrid_now[i] = (i + rank*(gridsize / num_processes));// % 3;
  if(rank == 0) {
    localgrid_now[0] = 0;
    localgrid_future[0] = 0;
  }
  if(rank == num_processes-1) {
    localgrid_now[localgridsize-1] = 0;
    localgrid_future[localgridsize-1] = 0;
  }

  for(int j = 0; j < num_iterations; j++) {
    // Do heat equation physics
    for(int i = 1; i < localgridsize-1; i++) {
      localgrid_future[i] = localgrid_now[i] + dt*alpha/(dx*dx) * (localgrid_now[i-1] - 2*localgrid_now[i] + localgrid_now[i+1]);
    }
    std::swap(localgrid_now, localgrid_future);

    // Exchange ghost cells with neighbouring processes
    if(rank > 0) {
      // Send first cell of subgrid to right-most ghost cell of left neighbour
      MPI_Send(localgrid_now+1, 1, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD);
      // Receive last cell of subgrid of left neighbour as our left-most ghost cell
      MPI_Recv(localgrid_now, 1, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    if(rank < num_processes-1) {
      // Receive first cell of subgrid of right neighbour as our right-most ghost cell
      MPI_Recv(localgrid_now+localgridsize-1, 1, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      // Send last cell of subgrid to left-most ghost cell of right neighbour
      MPI_Send(localgrid_now+localgridsize-2, 1, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD);
    }

    if(j % save_every == 0) {
      // Save data. Only process 0 does I/O.
      if(rank == 0) {
        std::ofstream out("heat_" + std::to_string(j / save_every) + ".dat");
        for(int i = 1; i < localgridsize-1; i++)
          out << localgrid_now[i] << '\n';

        const int maxsize = gridsize / num_processes + gridsize % num_processes;
        double *buffer = new double[maxsize];
        for(int i = 1; i < num_processes; i++) {
          int recv;
          MPI_Status status;
          MPI_Recv(buffer, maxsize, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
          MPI_Get_count(&status, MPI_DOUBLE, &recv);
          for(int k = 0; k < recv; k++)
            out << buffer[k] << '\n';
        }
        delete[] buffer;
        out.close();
      }
      else {
        MPI_Send(localgrid_now+1, localgridsize-2, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
      }
    }
  }

  delete[] localgrid_now;
  delete[] localgrid_future;

  if(rank == 0)
    std::cout << "Job took " << MPI_Wtime() - starttime << " seconds." << std::endl;

  MPI_Finalize();
}
