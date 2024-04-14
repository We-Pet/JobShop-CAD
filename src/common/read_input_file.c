int read_file_first_line(FILE *file_pointer, int *number_of_jobs, int *number_of_machines){
    if (!file_pointer){
        fprintf(stderr, "File does not exist.\n");
        return EXIT_FAILURE;
    }

    char line_buffer[10];
    if (fgets(line_buffer, sizeof(line_buffer), file_pointer)){
        sscanf(line_buffer, "%d %d", number_of_jobs, number_of_machines);
    }
    return EXIT_SUCCESS;
}

int read_file_matrix(FILE *file_pointer, struct Job *jobs){

    char line_buffer[256];

    int job_number = 0, machine_id = 0, duration = 0, operation_id = 0;
    int num1, num2;
    while (fscanf(file_pointer, "%d", &machine_id) != EOF) {
        fscanf(file_pointer, "%d", &duration);
        jobs[job_number].job_number = job_number;
        jobs[job_number].total_duration += duration;
        jobs[job_number].total_operations++;
        jobs[job_number].operations[operation_id].duration = duration;
        jobs[job_number].operations[operation_id].machine_number = machine_id;
        operation_id++;
        if (fgetc(file_pointer) == '\n' || feof(file_pointer)) {
            job_number++;
            operation_id = 0;
        }
    }

    fclose(file_pointer);
    return EXIT_SUCCESS;
}

