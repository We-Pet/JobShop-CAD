int check_sequence(struct Job job, struct Output_time *output_times)
{
    for (int i = 0; i < job.total_operations - 1; i++)
    {
        if (output_times[job.job_number].start_time_operations[i] + job.operations[i].duration > output_times[job.job_number].start_time_operations[i + 1])
        {
            return -1;
        }
    }

    return 0;
}