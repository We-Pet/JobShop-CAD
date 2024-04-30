void initialize_output_time(struct Output_time *output_time, int number_of_jobs, const struct Job *jobs) {
    for(int i = 0; i < number_of_jobs; i++){
        output_time[i].job_number = i;
        output_time[i].start_time_operations = malloc(sizeof(int) * jobs[i].total_operations);
    }
}
