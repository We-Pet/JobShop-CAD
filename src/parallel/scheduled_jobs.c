#include <pthread.h>

void* job_function(void* arg){
    struct Thread_Args* thread_args = (struct Thread_Args*)arg;
    struct Job job = thread_args->job;

    pthread_mutex_t* mutexes_machines = thread_args->mutexes_machines;
    int* machines = thread_args->machines;
    int* job_completion_times = thread_args->job_completion_times;
    int* start_time_operations = thread_args->start_time_operations;

    for (int i = 0; i < job.total_operations; i++){
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

void initialize_mutexes(pthread_mutex_t *mutexes_machines, int number_of_machines) {
    for (int i = 0; i < number_of_machines; i++)
        pthread_mutex_init(&mutexes_machines[i], NULL);
}

void initialize_output_time(struct Output_time *output_time, int number_of_jobs, struct Job *jobs) {
    for(int i = 0; i < number_of_jobs; i++){
        output_time[i].job_number = i;
        output_time[i].start_time_operations = malloc(sizeof(int) * jobs[i].total_operations);
    }
}

void write_output_file(struct Output_time *output_time, int number_of_jobs) {
    char output_file[50];
    sprintf(output_file, "output_files/parallel/ft_%d.jss", number_of_jobs);
    FILE *file_ptr = fopen(output_file, "w+");
    if (!file_ptr) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i<number_of_jobs; i++){
        for(int j=0; j < output_time[i].job_number; j++){
            fprintf(file_ptr, "%d ", output_time[i].start_time_operations[j]);
        }
        fprintf(file_ptr, "\n");
    }
    fclose(file_ptr);
}

void schedule_jobs(struct Job *jobs, int number_of_jobs, int number_of_machines, int num_threads){
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
    for (int i = 0; i < limit_iterations; i++){
        thread_args[i].job = jobs[i % number_of_jobs];
        thread_args[i].mutexes_machines = mutexes_machines;
        thread_args[i].machines = machines;
        thread_args[i].job_completion_times = job_completion_times;
        thread_args[i].number_of_jobs = number_of_jobs;
        thread_args[i].start_time_operations = output_time[i].start_time_operations;
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
            thread_args[threads_index].machines = machines;
            thread_args[threads_index].job_completion_times = job_completion_times;
            thread_args[threads_index].start_time_operations = output_time[current_job_index].start_time_operations;
            pthread_create(&threads[threads_index], NULL, job_function, (void*)&thread_args[threads_index]);
            threads_index++;
            current_job_index++;
            if (threads_index == num_threads) threads_index = 0;
        }
    }

    for (int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);
    clock_t time_after = clock();

    write_output_file(output_time, number_of_jobs);

    int make_span = 0;
    for (int i = 0; i < number_of_jobs; i++){
        make_span = (job_completion_times[i] > make_span) ? job_completion_times[i] : make_span;
    }
    pthread_mutex_destroy(mutexes_machines);

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
