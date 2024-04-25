#include <pthread.h>

int make_span = 0;

struct Thread_Args{
    struct Job job;
    const char output_file;
    pthread_mutex_t* mutexes_machines;
    pthread_mutex_t* mutexes_job_completion_times;
    int* machines;
    int* job_completion_times;
};

void* job_function(void* arg){
    struct Thread_Args* thread_args = (struct Thread_Args*)arg;
    struct Job job = thread_args->job;

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
        if (end_time > make_span)
            make_span = end_time;
        printf("Job %d (Machine %d): Start time = %d, End time = %d\n", job.job_number, machine_id, start_time, end_time);
    }
    return NULL;
}

void schedule_jobs(struct Job *jobs, int number_of_jobs, int number_of_machines, int num_threads){
    int machines[number_of_machines];
    int job_completion_times[number_of_jobs];

    char output_file[50];
    struct Thread_Args thread_args[number_of_jobs];

    pthread_mutex_t mutexes_machines[number_of_machines];
    pthread_mutex_t mutexes_job_completion_times[number_of_jobs];
    pthread_t threads[num_threads];

    sprintf(output_file, "output_files/parallel/ft_%d.jss", number_of_jobs);

    memset(machines, 0, sizeof(machines));
    memset(job_completion_times, 0, sizeof(job_completion_times));


    for (int i = 0; i < number_of_jobs; i++)
        pthread_mutex_init(&mutexes_job_completion_times[i], NULL);

    for (int i = 0; i < number_of_machines; i++)
        pthread_mutex_init(&mutexes_machines[i], NULL);

    // if num_threads is greater than number_of_jobs, only create as many threads as there are jobs.
    int limit_iterations = (num_threads > number_of_jobs) ? number_of_jobs : num_threads;
    for (int i = 0; i < limit_iterations; i++){
        thread_args[i].job = jobs[i % number_of_jobs];
        thread_args[i].mutexes_machines = mutexes_machines;
        thread_args[i].mutexes_job_completion_times = mutexes_job_completion_times;
        thread_args[i].machines = machines;
        thread_args[i].job_completion_times = job_completion_times;
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

    pthread_mutex_destroy(mutexes_job_completion_times);
    pthread_mutex_destroy(mutexes_machines);

    printf("Makespan: %d\n", make_span);
}
















// struct ThreadArgs {
//     struct Job job;
//     int total_jobs;
//     const char* output_file;
//     pthread_mutex_t* mutex;
// };

// void* job_thread(void* arg){
//     struct ThreadArgs* thread_args = (struct ThreadArgs*)arg;
//     struct Job job = thread_args->job;
//     int total_jobs = thread_args->total_jobs;
//     const char* output_file = thread_args->output_file;
//     FILE* file_ptr = fopen(output_file, "a");

//     for (int j = 0; j < job.total_operations; j++){
//         int machine_id = job.operations[j].machine_number;
//         int duration = job.operations[j].duration;
//         int start_time, end_time;

//         pthread_mutex_lock(&mutex);
//         if (machines[machine_id] > job_completion_times[job.job_number]) {
//             start_time = machines[machine_id];
//         } else {
//             start_time = job_completion_times[job.job_number];
//         }
//         pthread_mutex_unlock(&mutex);

//         end_time = start_time + duration;

//         pthread_mutex_lock(&mutex);
//         machines[machine_id] = end_time;
//         job_completion_times[job.job_number] = end_time;
//         if (end_time > make_span)
//             make_span = end_time;
//             printf("Job %d (Machine %d): Start time = %d, End time = %d\n", job.job_number, machine_id, start_time, end_time);
//         pthread_mutex_unlock(&mutex);

//         fprintf(file_ptr, "%d ", start_time);
//     }

//     fprintf(file_ptr, "\n");
//     fclose(file_ptr);

//     return NULL;
// }


// void schedule_jobs(struct Job *jobs, int number_of_jobs, int number_of_machines, int num_threads){
//     pthread_mutex_t* mutexes = (pthread_mutex_t*)malloc(number_of_machines * sizeof(pthread_mutex_t));
//     char output_file[50];
//     sprintf(output_file, "output_files/parallel/ft_%d.jss", number_of_jobs);

//     clock_t time_before = clock();
//     struct ThreadArgs thread_args[num_threads];
//     int current_job_index = 0;

//     for (int i = 0; i < number_of_machines; i++) {
//         pthread_mutex_init(&mutexes[i], NULL);
//     }

//     for (int i = 0; i < num_threads; i++){
//         if (current_job_index < number_of_jobs) {
//             thread_args[i].job = jobs[i % number_of_jobs];
//             thread_args[i].total_jobs = number_of_jobs;
//             thread_args[i].output_file = output_file;
//             pthread_create(&threads[i], NULL, job_thread, (void *)&thread_args[i]);
//             current_job_index++;
//         }
//     }

//     while (current_job_index < number_of_jobs){
//         for (int i =0; i < num_threads; i++){
//             if (current_job_index < number_of_jobs){
//                 thread_args[i].job = jobs[current_job_index];
//                 pthread_create(&threads[i], NULL, job_thread, (void *)&thread_args[i]);
//                 current_job_index++;
//             }
//         }
//     }

// int iteration_limit  = num_threads > number_of_jobs ? number_of_jobs : num_threads;
// for (int i = 0; i < iteration_limit; i++){
//     thread_args[i].job = jobs[i % number_of_jobs];
//     thread_args[i].total_jobs = number_of_jobs;
//     thread_args[i].output_file = output_file;
//     pthread_create(&threads[i], NULL, job_thread, (void*)&thread_args[i]);
// }

//     for (int i = 0; i < iteration_limit; i++){
//         pthread_join(threads[i], NULL);
//     }
//     clock_t time_after = clock();
//     double time_in_ms;
// Windows CLOCKS_PER_SEC is different from Linux CLOCKS_PER_SEC
//     #ifdef _WIN32
//         time_in_ms = (double)(time_after - time_before) * 10.0 / CLOCKS_PER_SEC;
//     #else
//         time_in_ms = (double)(time_after - time_before) * 1000.0 / CLOCKS_PER_SEC;
//     #endif

//     printf("Makespan: %d\n", make_span);
//     printf("Time: %.5f\n", time_in_ms);
// }
