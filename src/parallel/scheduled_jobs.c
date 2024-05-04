#include <pthread.h>

void *job_function(void *arg)
{
    struct Thread_Args *thread_args = (struct Thread_Args *)arg;
    struct Job job = thread_args->job;

    pthread_mutex_t *mutexes_machines = thread_args->mutexes_machines;
    int *machines = thread_args->machines;
    int *job_completion_times = thread_args->job_completion_times;
    int *start_time_operations = thread_args->start_time_operations;

    for (int i = 0; i < job.total_operations; i++)
    {
        int machine_number = job.operations[i].machine_number;
        int machine_id = job.operations[i].machine_number;
        int duration = job.operations[i].duration;
        pthread_mutex_lock(&mutexes_machines[machine_number]);
        int start_time = (machines[machine_id] > job_completion_times[job.job_number]) ? machines[machine_id] : job_completion_times[job.job_number];
        int end_time = start_time + duration;
        machines[machine_id] = end_time;
        pthread_mutex_unlock(&mutexes_machines[machine_number]);
        job_completion_times[job.job_number] = end_time;
        printf("Job %d (Machine %d): Start time = %d, End time = %d\n", job.job_number, machine_id, start_time, end_time);
        start_time_operations[i] = start_time;
    }
    return NULL;
}

void initialize_mutexes(pthread_mutex_t *mutexes_machines, int number_of_machines)
{
    for (int i = 0; i < number_of_machines; i++)
        pthread_mutex_init(&mutexes_machines[i], NULL);
}

void create_thread(int thread_index, pthread_t *threads, struct Thread_Args *thread_args, const struct Job *jobs, int num_threads, int current_job_index, struct Output_time *output_time, int *job_completion_times, pthread_mutex_t *mutexes_machines, int *machines, int number_of_jobs)
{
    thread_args[thread_index].job = jobs[current_job_index];
    thread_args[thread_index].mutexes_machines = mutexes_machines;
    thread_args[thread_index].machines = machines;
    thread_args[thread_index].job_completion_times = job_completion_times;
    thread_args[thread_index].number_of_jobs = number_of_jobs;
    thread_args[thread_index].start_time_operations = output_time[current_job_index].start_time_operations;
    pthread_create(&threads[thread_index], NULL, job_function, (void *)&thread_args[thread_index]);
}

void schedule_jobs(const struct Job *jobs, int number_of_jobs, int number_of_machines, int num_threads, char *output_file)
{
    int machines[number_of_machines];
    int job_completion_times[number_of_jobs];

    pthread_mutex_t mutexes_machines[number_of_machines];

    memset(machines, 0, sizeof(machines));
    memset(job_completion_times, 0, sizeof(job_completion_times));

    initialize_mutexes(mutexes_machines, number_of_machines);

    struct Thread_Args thread_args[number_of_jobs];
    struct Output_time output_time[number_of_jobs];

    initialize_output_time(output_time, number_of_jobs, jobs);

    // if num_threads is greater than number_of_jobs, only create as many threads as there are jobs.
    int limit_iterations = (num_threads > number_of_jobs) ? number_of_jobs : num_threads;
    pthread_t threads[limit_iterations];

    clock_t time_before = clock();
    for (int i = 0; i < limit_iterations; i++)
    {
        create_thread(i, threads, thread_args, jobs, num_threads, i, output_time, job_completion_times, mutexes_machines, machines, number_of_jobs);
    }

    int current_job_index = limit_iterations;
    if (current_job_index < number_of_jobs)
    {
        int threads_index = 0;
        while (current_job_index < number_of_jobs)
        {
            // wait for the thread in the threads_index to finish
            pthread_join(threads[threads_index], NULL);
            create_thread(threads_index, threads, thread_args, jobs, num_threads, current_job_index, output_time, job_completion_times, mutexes_machines, machines, number_of_jobs);
            threads_index++;
            current_job_index++;
            if (threads_index == limit_iterations)
                threads_index = 0;
        }
    }

    for (int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);
    clock_t time_after = clock();

    pthread_mutex_destroy(mutexes_machines);

    FILE *file_ptr = fopen(output_file, "w+");
    if (!file_ptr)
    {
        perror("Error opening file");
        return;
    }

    int make_span = 0;
    for (int i = 0; i < number_of_jobs; i++)
    {
        make_span = (job_completion_times[i] > make_span) ? job_completion_times[i] : make_span;
        for (int j = 0; j < jobs[i].total_operations; j++)
        {
            fprintf(file_ptr, "%d ", output_time[i].start_time_operations[j]);
        }
        fprintf(file_ptr, "\n");
    }
    fclose(file_ptr);

    printf("Makespan: %d\n", make_span);
    double time_in_ms;
// Windows CLOCKS_PER_SEC is different from Linux CLOCKS_PER_SEC
#ifdef _WIN32
    time_in_ms = (double)(time_after - time_before) * 10.0 / CLOCKS_PER_SEC;
#else
    time_in_ms = (double)(time_after - time_before) * 1000.0 / CLOCKS_PER_SEC;
#endif

    printf("Time: %.5f\n", time_in_ms);
}
