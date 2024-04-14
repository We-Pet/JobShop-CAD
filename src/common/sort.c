int compare_operations(const void *a, const void *b){
    struct Operation *op1 = (struct Operation *)a;
    struct Operation *op2 = (struct Operation *)b;
    return (op1->duration > op2->duration) - (op1->duration < op2->duration);
}

int compare_jobs(const void *a, const void *b) {
    const struct Job *job1 = (const struct Job *)a;
    const struct Job *job2 = (const struct Job *)b;
    return job1->total_duration - job2->total_duration;
}

void sort_jobs_array(struct Job *jobs, int number_of_jobs){
    for (int i = 0; i < number_of_jobs; i++){
        qsort(jobs[i].operations, jobs[i].total_operations, sizeof(struct Operation), compare_operations);
    }
    qsort(jobs, number_of_jobs, sizeof(struct Job), compare_jobs);
}
