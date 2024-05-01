int check_sequence(struct Job job, struct Output_time *output_times)
{
    // print job operations and their start time
    printf("Job %d\n", job.job_number);
    for (int i = 0; i < job.total_operations; i++)
    {
        printf("Operation %d: %d\n", i, output_times[job.job_number].start_time_operations[i]);
        printf("Operation %d duration: %d\n", i, job.operations[i].duration);
    }

    for (int i = 0; i < job.total_operations - 1; i++)
    {
        if (output_times[job.job_number].start_time_operations[i] + job.operations[i].duration > output_times[job.job_number].start_time_operations[i + 1])
        {
            return -1;
        }
    }
    return 0;
}