int read_file_first_line(FILE *file_pointer, int *number_of_jobs, int *number_of_machines)
{
    if (!file_pointer)
    {
        fprintf(stderr, "File does not exist.\n");
        return EXIT_FAILURE;
    }

    char line_buffer[10];
    // Read the first line to the line_buffer and then parse it to number_of_jobs and number_of_machines
    if (fgets(line_buffer, sizeof(line_buffer), file_pointer))
    {
        sscanf(line_buffer, "%d %d", number_of_jobs, number_of_machines);
    }
    return EXIT_SUCCESS;
}

int read_file_matrix(FILE *file_pointer, struct Job *jobs)
{

    int job_number = 0, machine_id = 0, duration = 0, operation_id = 0;
    int num1, num2;
    // Read file until finding a integer, and jump to the next integer (read the machine and jump to the duration)
    while (fscanf(file_pointer, "%d", &machine_id) != EOF)
    {
        // Read file until finding a integer, and jump to the next integer (read the duration and jump to the next machine)
        fscanf(file_pointer, "%d", &duration);

        jobs[job_number].job_number = job_number;
        jobs[job_number].total_operations++;
        jobs[job_number].operations[operation_id].duration = duration;
        jobs[job_number].operations[operation_id].machine_number = machine_id;
        operation_id++;

        // Read the file until finding a char (\n) or the end of line, this means the end of the job, reset the operation_id and jump to the next job
        if (fgetc(file_pointer) == '\n' || feof(file_pointer))
        {
            job_number++;
            operation_id = 0;
        }
    }

    fclose(file_pointer);
    return EXIT_SUCCESS;
}
