void schedule_jobs(struct Job *jobs, int number_of_jobs, int number_of_machines)
{
    int machines[number_of_machines];
    int job_completion_times[number_of_jobs];
    int make_span = 0;

    memset(machines, 0, sizeof(machines));
    memset(job_completion_times, 0, sizeof(job_completion_times));

    clock_t time_before = clock();
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
            printf("Job %d (Machine %d): Start time = %d, End time = %d\n", jobs[i].job_number, machine_id, start_time, end_time);
        }
    }
    clock_t time_after = clock();
    double time_in_ms;
    // Windows CLOCKS_PER_SEC is different from Linux CLOCKS_PER_SEC
    #ifdef _WIN32
        time_in_ms = (double)(time_after - time_before) * 10.0 / CLOCKS_PER_SEC;
    #else
        time_in_ms = (double)(time_after - time_before) * 1000.0 / CLOCKS_PER_SEC;
    #endif

    printf("Makespan: %d\n", make_span);
    printf("Time: %.5f\n", time_in_ms);
}
