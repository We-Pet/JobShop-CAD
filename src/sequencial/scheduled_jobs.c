void schedule_jobs(struct Job *jobs, int number_of_jobs, int number_of_machines, struct Output_time *output_time, char *output_file, int executionsNumber)
{
    double elapsed;
    int best_make_span = INT_MAX;
    for (int currExecution = 0; currExecution < executionsNumber; currExecution++)
    {
        double elapsedCurrent = 0;
        int machines[number_of_machines];
        int job_completion_times[number_of_jobs];
        int make_span = 0;

        memset(machines, 0, sizeof(machines));
        memset(job_completion_times, 0, sizeof(job_completion_times));

        clock_t time_before = clock();
        clock_gettime(CLOCK_MONOTONIC, &start);
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
                output_time[i].start_time_operations[j] = start_time;

                for (int counter = 0; counter < 100000; counter++)
                    ;
            }
        }
        clock_t time_after = clock();
        clock_gettime(CLOCK_MONOTONIC, &finish);

        for (int i = 0; i < number_of_jobs; i++)
        {
            make_span = (job_completion_times[i] > make_span) ? job_completion_times[i] : make_span;
        }

        if (make_span < best_make_span)
        {
            FILE *file_ptr = fopen(output_file, "w+");
            best_make_span = make_span;
            for (int i = 0; i < number_of_jobs; i++)
            {
                for (int j = 0; j < jobs[i].total_operations; j++)
                {
                    fprintf(file_ptr, "%d ", output_time[i].start_time_operations[j]);
                }
                fprintf(file_ptr, "\n");
            }
            fclose(file_ptr);
        }

        double time_in_ms;
// Windows CLOCKS_PER_SEC is different from Linux CLOCKS_PER_SEC
#ifdef _WIN32
        time_in_ms = (double)(time_after - time_before) * 10.0 / CLOCKS_PER_SEC;
#else
        time_in_ms = (double)(time_after - time_before) * 1000.0 / CLOCKS_PER_SEC;
#endif

        elapsedCurrent = (finish.tv_sec - start.tv_sec);
        elapsedCurrent += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

        elapsed += elapsedCurrent;
    }
    printf("Makespan: %d\n", best_make_span);
    printf("Elapsed Time: %.5fs\n", elapsed / executionsNumber);
}
