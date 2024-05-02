#include <stdio.h>

#include "../common/includes.h"
#include "check_sequence.c"
int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Bad arguments: provide file name");
        return EXIT_FAILURE;
    }

    FILE *file_pointer = fopen(argv[1], "r");
    FILE *output_file_pointer = fopen(argv[2], "r");

    int number_of_jobs = 0, number_of_machines = 0;
    read_file_first_line(file_pointer, &number_of_jobs, &number_of_machines);
    printf("Number of jobs: %d\n", number_of_jobs);
    struct Job jobs[number_of_jobs];
    for (int i = 0; i < number_of_jobs; i++)
        jobs[i].total_operations = 0;

    read_file_matrix(file_pointer, jobs);

    struct Output_time *output_times = read_output_file(output_file_pointer, number_of_jobs);
    int valid = 1;

    for (int i = 0; i < number_of_jobs; i++)
    {
        int number_of_operations = jobs[i].total_operations;

        // Check sequence
        for (int j = 0; j < number_of_operations - 1; j++)
        {
            if (check_sequence(jobs[i], output_times) == -1)
            {
                printf("\nInvalid sequence on job %d\n", i);
                valid = 0;
                break;
            }
        }
        printf("\n");
    }

    if (valid != 1)
    {
        printf("Output is invalid\n");
    }
    else
    {
        printf("Output is valid\n");
    }

    fclose(file_pointer);
    fclose(output_file_pointer);
    return EXIT_SUCCESS;
}
