#define MAX_OPERATIONS 100

struct Operation{
    int machine_number;
    int duration;
};

// One job as x operations, operations = job * machine
struct Job{
    int job_number;
    int total_duration;
    int total_operations;
    struct Operation operations[MAX_OPERATIONS];
};
