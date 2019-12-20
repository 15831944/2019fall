#include <omp.h>
#include <iostream>

#define NUM_THREADS 8
using namespace std;

void main()
{
	omp_set_num_threads(NUM_THREADS);
    #pragma omp parallel
	{
		#pragma omp master
		{
			printf("teach, master.\n");
		}
		int tid = omp_get_thread_num(); 
		printf("learn, tid = %d\n", tid);
        #pragma omp single
		{
			printf("ask question, tid = %d\n", tid);
		}
	}
}

