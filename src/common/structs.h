#define MAX_OPERATIONS 2000

struct Operation{
    int machine_number;
    int duration;
};

// One job as x operations, operations = job * machine
struct Job{
    int job_number;
    int total_operations;
    struct Operation operations[MAX_OPERATIONS];
};

struct Output_time{
    int job_number;
    int *start_time_operations;
};

struct Thread_Args{
    struct Job job;
    int *start_time_operations;
    pthread_mutex_t* mutexes_machines;
    int* machines;
    int* job_completion_times;
    int number_of_jobs;
};

struct timespec start, finish;
