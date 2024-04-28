void schedule_jobs(struct Job *jobs, int number_of_jobs, int number_of_machines)
{
    int machines[number_of_machines];
    int job_completion_times[number_of_jobs];
    int make_span = 0;

    char output_file[50];
    sprintf(output_file, "output_files/sequencial/ft_%d.jss", number_of_jobs);
    FILE* file_ptr = fopen(output_file, "w+");


    memset(machines, 0, sizeof(machines));
    memset(job_completion_times, 0, sizeof(job_completion_times));

    gettimeofday(&start_time, NULL);
    for (int i = 0; i < number_of_jobs; i++)
    {
        for (int j = 0; j < jobs[i].total_operations; j++)
        {
            int machine_id = jobs[i].operations[j].machine_number;
            int duration = jobs[i].operations[j].duration;
            int start_time = (machines[machine_id] > job_completion_times[i]) ? machines[machine_id] : job_completion_times[i];
            int end_time = start_time + duration;
            machines[machine_id] = end_time;
            job_completion_times[i] = end_time;
            if (end_time > make_span)
                make_span = end_time;
            fprintf(file_ptr, "%d ", start_time);
        }
        fprintf(file_ptr, "\n");
    }
    gettimeofday(&end_time, NULL);
    fclose(file_ptr);
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000.0 + (end_time.tv_usec - start_time.tv_usec) / 1000.0;
    double elapsed_time_in_seconds = elapsed_time / 1000.0;

    printf("Makespan: %d\n", make_span);
    printf("Time: %.5fs\n", elapsed_time_in_seconds);
}
