#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>

int main(int argc, char **argv)
{

    Args ins__args;
    parseArgs(&ins__args, &argc, argv);
    long input = ins__args.arg;

    struct timeval ins__tstart, ins__tstop;

    int myrank, nproc;

    MPI_Init(&argc, &argv);

    // obtain my rank
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    // and the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    if (!myrank)
        gettimeofday(&ins__tstart, NULL);

    int isPrime = 0, isPrimeFinal = 0;

    printf("\n %ld", input);

    for (long i = myrank + 2; i * i <= input / 2; i += nproc)
    {
        if (input % i == 0)
        {
            isPrime = 1;
            break;
        }
    }

    // synchronize
    MPI_Reduce(&isPrime, &isPrimeFinal, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (!myrank)
    {
        if (isPrimeFinal == 0)
        {
            printf("\n %ld is primal", input);
        }
        else
        {
            printf("\n %ld isn't primal", input);
        }
        gettimeofday(&ins__tstop, NULL);
        ins__printtime(&ins__tstart, &ins__tstop, ins__args.marker);
    }

    MPI_Finalize();
}
