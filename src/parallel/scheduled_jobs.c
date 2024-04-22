#include <pthread.h>

int machines[3];
int job_completion_times[3];
int make_span = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct ThreadArgs {
    struct Job job;
    int total_jobs;
    const char* output_file;
};

void* job_thread(void* arg){
    struct ThreadArgs* thread_args = (struct ThreadArgs*)arg;
    struct Job job = thread_args->job;
    int total_jobs = thread_args->total_jobs;
    const char* output_file = thread_args->output_file;
    FILE* file_ptr = fopen(output_file, "a");

    for (int j = 0; j < job.total_operations; j++){
        int machine_id = job.operations[j].machine_number;
        int duration = job.operations[j].duration;
        int start_time, end_time;

        pthread_mutex_lock(&mutex);
        start_time = (machines[machine_id] > job_completion_times[job.job_number]) ? machines[machine_id] : job_completion_times[job.job_number];
        end_time = start_time + duration;
        machines[machine_id] = end_time;
        job_completion_times[job.job_number] = end_time;
        if (end_time > make_span)
            make_span = end_time;

        fprintf(file_ptr, "%d ", start_time);

        pthread_mutex_unlock(&mutex);
    }

    fprintf(file_ptr, "\n");
    fclose(file_ptr);

    return NULL;
}


void schedule_jobs(struct Job *jobs, int number_of_jobs, int number_of_machines, int num_threads){
    pthread_t threads[num_threads];

    char output_file[50];
    sprintf(output_file, "output_files/parallel/ft_%d.jss", number_of_jobs);

    clock_t time_before = clock();
    struct ThreadArgs thread_args[num_threads];
    for (int i = 0; i < num_threads; i++){
        thread_args[i].job = jobs[i % number_of_jobs];
        thread_args[i].total_jobs = number_of_jobs;
        thread_args[i].output_file = output_file;
        pthread_create(&threads[i], NULL, job_thread, (void*)&thread_args[i]);
    }

    for (int i = 0; i < num_threads; i++){
        pthread_join(threads[i], NULL);
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
