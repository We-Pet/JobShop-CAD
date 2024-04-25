#include "../common/includes.h"
#include "scheduled_jobs.c"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Bad arguments: %s <file> <n_threads>\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE *file_pointer = fopen(argv[1], "r");
  if (file_pointer == NULL) {
    fprintf(stderr, "Error opening the file\n");
    return EXIT_FAILURE;
  }

  int number_of_threads = atoi(argv[2]);
  if (number_of_threads <= 0){
    fprintf(stderr, "Threads number must be > 0\n");
    return EXIT_FAILURE;
  }

  // Get number os jobs and number of machines from first line
  int number_of_jobs = 0, number_of_machines = 0;
  read_file_first_line(file_pointer, &number_of_jobs, &number_of_machines);

  struct Job jobs[number_of_jobs];

  for (int i = 0; i < number_of_jobs; i++) {
    jobs[i].total_duration = 0;
    jobs[i].total_operations = 0;
  }

  read_file_matrix(file_pointer, jobs);

  printf("Scheduled Jobs:\n");
  schedule_jobs(jobs, number_of_jobs, number_of_machines, number_of_threads);
  fclose(file_pointer);
  return EXIT_SUCCESS;
}
