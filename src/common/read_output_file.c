struct Output_time *read_output_file(FILE *file_pointer, int number_of_jobs)
{
    if (file_pointer == NULL)
    {
        fprintf(stderr, "Error opening the file\n");
        return NULL;
    }
    struct Output_time *jobs = malloc(number_of_jobs * sizeof(struct Output_time));

    if (!jobs)
    {
        perror("Failed to allocate memory for jobs");
        fclose(file_pointer);
        return NULL;
    }

    for (int i = 0; i < number_of_jobs; i++)
    {
        jobs[i].job_number = i;
        jobs[i].start_time_operations = malloc(number_of_jobs * sizeof(int));
        if (!jobs[i].start_time_operations)
        {
            perror("Failed to allocate memory for start times");
            for (int j = 0; j < i; j++)
            {
                free(jobs[j].start_time_operations);
            }
            free(jobs);
            fclose(file_pointer);
            return NULL;
        }
    }

    char line[256];
    int current_job = 0;

    while (fgets(line, sizeof(line), file_pointer) && current_job < number_of_jobs)
    {
        char *token = strtok(line, " \t\n");
        int op_count = 0;

        while (token && op_count < number_of_jobs)
        {
            jobs[current_job].start_time_operations[op_count] = atoi(token);
            token = strtok(NULL, " \t\n");
            op_count++;
        }
        current_job++;
    }

    fclose(file_pointer);
    return jobs;
}