#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <mpi.h>

int main(int argc, char *argv[]) {
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

  std::mt19937 generator;
  std::uniform_real_distribution<double> dist(0, 1);

  // Seed needs rank, otherwise all MPI processes will generate the same pseudo-random numbers.
  generator.seed(rank);

  int num_steps = 1000;
  int num_trials = 1000;
  int num_experiments = 50;
  double stepsize = 1.;
  double *local_distance = new double[num_experiments];

  int local_num_trials = num_trials / num_processes;

  for(int k = 0; k < num_experiments; k++) {
    local_distance[k] = 0;
    for(int j = 0; j < local_num_trials; j++) {
      double x = 0, y = 0, z = 0;
      for(int i = 0; i < (k+1)*num_steps; i++) {
        // Generate random 3D angle
        double phi = 2*M_PI*dist(generator);
        double theta = acos(2*dist(generator) - 1);

        // Take a step in the random direction
        x += stepsize * cos(phi) * sin(theta);
        y += stepsize * sin(phi) * sin(theta);
        z += stepsize * cos(theta);

      }
      local_distance[k] += sqrt(x*x + y*y + z*z);
    }
    local_distance[k] /= local_num_trials;
  }

  double *global_distance = nullptr;
  if(rank == 0)
    global_distance = new double[num_experiments];

  // Add results from all processes
  MPI_Reduce(local_distance, global_distance, num_experiments, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if(rank == 0) {
    std::ofstream fout("output.dat");
    fout.precision(16);
    fout.setf(std::fstream::scientific);
    for(int k = 0; k < num_experiments; k++) {
      fout << (k+1)*num_steps << '\t' << sqrt((k+1)*num_steps) << '\t' << global_distance[k] / num_processes << '\n';
    }
    delete[] global_distance;
    fout.close();
  }
  delete[] local_distance;

  if(rank == 0)
    std::cout << "Job took " << MPI_Wtime() - starttime << " seconds." << std::endl;

  MPI_Finalize();

  return 0;
}
