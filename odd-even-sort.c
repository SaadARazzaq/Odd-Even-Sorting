#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/**
 * Compare and split operation to merge two sorted arrays.
 *
 * @param nlocal    The number of elements in the local array.
 * @param elmnts    The local array of elements.
 * @param relmnts   The received array of elements to be merged with local elements.
 * @param wspace    Working space array to store the merged elements.
 * @param keepsmall Flag indicating whether to keep the smaller elements (1) or larger elements (0).
 */
void compareSplit(int nlocal, int *elmnts, int *relmnts, int *wspace, int keepsmall) {
    int i = 0, j = 0, k = 0;

    // Merge elements from elmnts and relmnts into wspace based on the specified order
    while (i < nlocal && j < nlocal) {
        // Compare elements and choose the smaller or larger element based on keepsmall flag
        if ((keepsmall && elmnts[i] < relmnts[j]) || (!keepsmall && elmnts[i] >= relmnts[j])) {
            wspace[k++] = elmnts[i++];
        } else {
            wspace[k++] = relmnts[j++];
        }
    }

    // Copy any remaining elements from elmnts
    while (i < nlocal) {
        wspace[k++] = elmnts[i++];
    }

    // Copy any remaining elements from relmnts
    while (j < nlocal) {
        wspace[k++] = relmnts[j++];
    }

    // Copy merged elements back to elmnts
    for (i = 0; i < nlocal; i++) {
        elmnts[i] = wspace[i];
    }
}

int compare(const void *e1, const void *e2) {
    return (*((int *)e1) - *((int *)e2));
}

int main(int argc, char *argv[]) {
    int n, npes, myrank, nlocal, evenrank, oddrank, i;
    int *elmnts, *relmnts, *wspace;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if (argc != 2) {
        if (myrank == 0) {
            printf("Usage: %s <number_of_elements>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    n = atoi(argv[1]);
    nlocal = n / npes;

    elmnts = (int *)malloc(nlocal * sizeof(int));
    relmnts = (int *)malloc(nlocal * sizeof(int));
    wspace = (int *)malloc(nlocal * sizeof(int));

    srand(myrank);
    for (i = 0; i < nlocal; i++)
        elmnts[i] = rand();

    qsort(elmnts, nlocal, sizeof(int), compare);

    // Determine the rank of the processors that myrank needs to communicate during the odd and even phases of the algorithm
    if (myrank % 2 == 0) {
        oddrank = myrank - 1;
        evenrank = myrank + 1;
    } else {
        oddrank = myrank + 1;
        evenrank = myrank - 1;
    }

    // Set the ranks at the end of the linear process
    if (oddrank < 0 || oddrank >= npes)
        oddrank = MPI_PROC_NULL;
    if (evenrank < 0 || evenrank >= npes)
        evenrank = MPI_PROC_NULL;

    for (i = 0; i < npes - 1; i++) {
        if (i % 2 == 1) /* Odd phase */
            MPI_Sendrecv(elmnts, nlocal, MPI_INT, oddrank, 1, relmnts, nlocal, MPI_INT, oddrank, 1, MPI_COMM_WORLD, &status);
        else /* Even phase */
            MPI_Sendrecv(elmnts, nlocal, MPI_INT, evenrank, 1, relmnts, nlocal, MPI_INT, evenrank, 1, MPI_COMM_WORLD, &status);

        compareSplit(nlocal, elmnts, relmnts, wspace, myrank < status.MPI_SOURCE);
    }

    free(elmnts);
    free(relmnts);
    free(wspace);
    MPI_Finalize();

    return 0;
}
