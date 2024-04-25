#include <pthread.h>

int make_span = 0;
pthread_mutex_t make_span_mutex = PTHREAD_MUTEX_INITIALIZER;

struct Thread_Args{
    struct Job job;
    pthread_mutex_t* mutexes_machines;
    pthread_mutex_t* mutexes_job_completion_times;
    int* machines;
    int* job_completion_times;
    int number_of_jobs;
};

void* job_function(void* arg){
    struct Thread_Args* thread_args = (struct Thread_Args*)arg;
    struct Job job = thread_args->job;

    char output_file[50];
    sprintf(output_file, "output_files/parallel/ft_%d.jss", thread_args->number_of_jobs);
    FILE *file_ptr = fopen(output_file, "a");
    if (!file_ptr) {
        perror("Error opening file");
        return NULL;
    }

    pthread_mutex_t* mutexes_job_completion_times = thread_args->mutexes_job_completion_times;
    pthread_mutex_t* mutexes_machines = thread_args->mutexes_machines;
    int* machines = thread_args->machines;
    int* job_completion_times = thread_args->job_completion_times;

    for (int i = 0; i < job.total_operations; i++){
        int machine_number = job.operations[i].machine_number;
        int machine_id = job.operations[i].machine_number;
        int duration = job.operations[i].duration;
        pthread_mutex_lock(&mutexes_machines[machine_number]);
        pthread_mutex_lock(&mutexes_job_completion_times[job.job_number]);
        int start_time = (machines[machine_id] > job_completion_times[job.job_number]) ? machines[machine_id] : job_completion_times[job.job_number];
        int end_time = start_time + duration;
        machines[machine_id] = end_time;
        pthread_mutex_unlock(&mutexes_machines[machine_number]);
        job_completion_times[job.job_number] = end_time;
        pthread_mutex_unlock(&mutexes_job_completion_times[job.job_number]);
        pthread_mutex_lock(&make_span_mutex);
        if (end_time > make_span)
            make_span = end_time;
        pthread_mutex_unlock(&make_span_mutex);
        printf("Job %d (Machine %d): Start time = %d, End time = %d\n", job.job_number, machine_id, start_time, end_time);
        fprintf(file_ptr, "%d ", start_time);
    }
    fprintf(file_ptr, "\n");
    fclose(file_ptr);
    return NULL;
}

void schedule_jobs(struct Job *jobs, int number_of_jobs, int number_of_machines, int num_threads){
    int machines[number_of_machines];
    int job_completion_times[number_of_jobs];

    struct Thread_Args thread_args[number_of_jobs];

    pthread_mutex_t mutexes_machines[number_of_machines];
    pthread_mutex_t mutexes_job_completion_times[number_of_jobs];
    pthread_t threads[num_threads];

    memset(machines, 0, sizeof(machines));
    memset(job_completion_times, 0, sizeof(job_completion_times));


    for (int i = 0; i < number_of_jobs; i++)
        pthread_mutex_init(&mutexes_job_completion_times[i], NULL);

    for (int i = 0; i < number_of_machines; i++)
        pthread_mutex_init(&mutexes_machines[i], NULL);

    // if num_threads is greater than number_of_jobs, only create as many threads as there are jobs.
    int limit_iterations = (num_threads > number_of_jobs) ? number_of_jobs : num_threads;

    clock_t time_before = clock();
    for (int i = 0; i < limit_iterations; i++){
        thread_args[i].job = jobs[i % number_of_jobs];
        thread_args[i].mutexes_machines = mutexes_machines;
        thread_args[i].mutexes_job_completion_times = mutexes_job_completion_times;
        thread_args[i].machines = machines;
        thread_args[i].job_completion_times = job_completion_times;
        thread_args[i].number_of_jobs = number_of_jobs;
        pthread_create(&threads[i], NULL, job_function, (void*)&thread_args[i]);
    }

    int current_job_index = num_threads;
    if (current_job_index < number_of_jobs){
        int threads_index = 0;
        while (current_job_index < number_of_jobs){
            // wait for the thread in the threads_index to finish
            pthread_join(threads[threads_index], NULL);
            thread_args[threads_index].job = jobs[current_job_index];
            thread_args[threads_index].job = jobs[current_job_index];
            thread_args[threads_index].mutexes_machines = mutexes_machines;
            thread_args[threads_index].mutexes_job_completion_times = mutexes_job_completion_times;
            thread_args[threads_index].machines = machines;
            thread_args[threads_index].job_completion_times = job_completion_times;
            pthread_create(&threads[threads_index], NULL, job_function, (void*)&thread_args[threads_index]);
            threads_index++;
            current_job_index++;
            if (threads_index == num_threads) threads_index = 0;
        }
    }

    for (int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);
    clock_t time_after = clock();

    double time_in_ms;
    // Windows CLOCKS_PER_SEC is different from Linux CLOCKS_PER_SEC
    #ifdef _WIN32
        time_in_ms = (double)(time_after - time_before) * 10.0 / CLOCKS_PER_SEC;
    #else
        time_in_ms = (double)(time_after - time_before) * 1000.0 / CLOCKS_PER_SEC;
    #endif

    pthread_mutex_destroy(mutexes_job_completion_times);
    pthread_mutex_destroy(mutexes_machines);
    pthread_mutex_destroy(&make_span_mutex);

    printf("Makespan: %d\n", make_span);
    printf("Time: %.5f\n", time_in_ms);

}
