#include "../common/includes.h"
#include "scheduled_jobs.c"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Bad arguments: %s <input_file> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file_pointer = fopen(argv[1], "r");
    if (file_pointer == NULL)
    {
        fprintf(stderr, "Error opening the file\n");
        return EXIT_FAILURE;
    }

    char* output_file = argv[2];

    // Get number os jobs and number of machines from first line
    int number_of_jobs = 0, number_of_machines = 0;
    read_file_first_line(file_pointer, &number_of_jobs, &number_of_machines);

    struct Output_time output_time[number_of_jobs];
    struct Job jobs[number_of_jobs];
    for (int i = 0; i < number_of_jobs; i++)
        jobs[i].total_operations = 0;

    read_file_matrix(file_pointer, jobs);

    initialize_output_time(output_time, number_of_jobs, jobs);

    schedule_jobs(jobs, number_of_jobs, number_of_machines, output_time, output_file);

    fclose(file_pointer);
    return EXIT_SUCCESS;
}
