#include "../common/includes.h"

void schedule_jobs(struct Job *jobs, int number_of_jobs, int number_of_machines){
    int machines[number_of_machines];
    int job_completion_times[number_of_jobs];
    int make_span = 0;

    memset(machines, 0, sizeof(machines));
    memset(job_completion_times, 0, sizeof(job_completion_times));

    for (int i = 0; i < number_of_jobs; i++){
        for (int j = 0; j < jobs[i].total_operations; j++){
            int machine_id = jobs[i].operations[j].machine_number;
            int duration = jobs[i].operations[j].duration;
            int start_time = (machines[machine_id] > job_completion_times[i]) ? machines[machine_id] : job_completion_times[i];
            int end_time = start_time + duration;
            machines[machine_id] = end_time;
            job_completion_times[i] = end_time;
            if (end_time > make_span) make_span = end_time;
            printf("Job %d (Machine %d): Start time = %d, End time = %d\n", jobs[i].job_number, machine_id, start_time, end_time);
        }
    }
    printf("Makespan: %d\n", make_span);
}

int main(int argc, char *argv[]) {
    if (argc != 2){
        fprintf(stderr, "Bad arguments: %s <file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file_pointer = fopen(argv[1], "r");
    if (file_pointer == NULL) {
        fprintf(stderr, "Error opening the file\n");
        return EXIT_FAILURE;
    }
    int number_of_jobs = 0, number_of_machines = 0;
    read_file_first_line(file_pointer, &number_of_jobs, &number_of_machines);

    struct Job jobs[number_of_jobs];
    for(int i=0; i<number_of_jobs; i++){
        jobs[i].total_duration = 0;
        jobs[i].total_operations = 0;
    }

    read_file_matrix(file_pointer, jobs);
    sort_jobs_array(jobs, number_of_jobs);

    printf("Scheduled Jobs:\n");
    schedule_jobs(jobs, number_of_jobs, number_of_machines);

    fclose(file_pointer);
    return EXIT_SUCCESS;
}
