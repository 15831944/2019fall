#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
double local_start, local_finish, local_elapsed, elapsed, test_start, test_finish, test_elapsed, test_final, t_delta;
int required, provided, my_rank, comm_sz, i, phase, thread_count, num_approximations, num_particles, 
    local_num_particles, dimensions, send_to, recv_from;
char *ptr;

required = MPI_THREAD_SINGLE;
MPI_Init_thread(&argc, &argv, required, &provided); 
MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
MPI_Barrier(MPI_COMM_WORLD);

thread_count = strtol(argv[1], &ptr, 10);
num_particles = strtol(argv[2], &ptr, 10);
t_delta = strtod(argv[3], &ptr);
num_approximations = strtol(argv[4], &ptr, 10);

local_num_particles = num_particles/comm_sz;
float *temp_forces_positions = malloc(2 * dimensions * local_num_particles * sizeof(float));
for(i = 0; i < 2 * dimensions * local_num_particles; i++)
    temp_forces_positions[i] = 0;

if(my_rank == 0)
    printf("START\n\n");

send_to = (my_rank + 1) % comm_sz;
recv_from = (my_rank + comm_sz - 1) % comm_sz;

MPI_Barrier(MPI_COMM_WORLD);
local_start = MPI_Wtime();

#pragma omp parallel num_threads(thread_count) private(i, phase)
{   
    for(i = 0; i < num_approximations; i++)
    {                                   
        for(phase = 0; phase < comm_sz; phase++)
        {
            #pragma omp single
            {
                test_start = MPI_Wtime();
                /* PERFORMANCE ISSUES START HERE */
                MPI_Sendrecv_replace(temp_forces_positions, local_num_particles * dimensions * 2, MPI_FLOAT, send_to, 0, recv_from, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  
                /* PERFORMANCE ISSUES END HERE */
                test_finish = MPI_Wtime();
                test_elapsed += (test_finish - test_start);                                         
            }
        }       
    }
}

MPI_Barrier(MPI_COMM_WORLD);
local_finish = MPI_Wtime(); 
local_elapsed = local_finish - local_start;
MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
MPI_Reduce(&test_elapsed, &test_final, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

if(my_rank == 0)
{       
    printf("FINISHED\n\n");
    printf("Time taken total: %lf seconds\n", elapsed);
    printf("Time taken for communication: %lf seconds\n", test_final);
}
MPI_Finalize();
return 0;
}
