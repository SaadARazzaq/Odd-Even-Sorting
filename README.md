# Odd-Even-Sorting

## Introduction

This MPI (Message Passing Interface) program implements the Odd-Even Sort algorithm to efficiently sort an array of integers in parallel across multiple processes. The Odd-Even Sort is a parallel sorting algorithm that works by iteratively comparing and swapping neighboring elements.

## Compilation

To compile the program, use the following command:

```bash
mpicc -o odd_even_sort odd_even_sort.c -lm
```

Here,

- mpicc is the MPI compiler wrapper.
- -o odd_even_sort specifies the output executable name.
- odd_even_sort.c is the source code file.
- -lm is used to link the math library (required for rand() function).

## Execution

To execute the program, use the mpirun command:

```bash
mpirun -np <num_processes> ./odd_even_sort <number_of_elements>
```

Here,

- <num_processes> is the number of MPI processes you want to run.
- <number_of_elements> is the total number of elements to be sorted.

For example, to run the program with 4 MPI processes and 1000 elements:

```bash
mpirun -np 4 ./odd_even_sort 1000
```

## Program Logic

1. Each MPI process generates a local array of random integers.
2. The local arrays are sorted individually using qsort.
3. Processes exchange elements during Odd and Even phases using MPI communication.
4. The compareSplit function is used to merge the elements based on the process rank.
5. The sorting process is repeated for a number of iterations equal to the number of processes.

**Note**

**- Ensure that MPI is properly installed on your system.**

**- Adjust the compilation and execution commands based on your MPI setup.**

**- The program expects a command-line argument for the number of elements.**

## Example Compilation:

```bash
mpicc -o odd_even_sort odd_even_sort.c -lm
mpirun -np 4 ./odd_even_sort 1000
```

This will run the program with 4 MPI processes, sorting an array of 1000 elements.

Feel free to modify the program or instructions based on your specific requirements and environment.

```bash 
Made with 💖 by Saad and Wasif
```
