import random

def generate_matrix(num_jobs, num_machines):
    matrix = []
    for _ in range(num_jobs):
        row = []
        used_machines = set()
        for _ in range(num_machines):
            machine = random.randint(0, num_machines - 1)
            while machine in used_machines:
                machine = random.randint(0, num_machines - 1)
            used_machines.add(machine)
            time = random.randint(1, 10)
            row.append((machine, time))
        matrix.append(row)
    return matrix

def write_matrix_to_file(matrix, filename):
    with open(filename, 'w') as f:
        f.write(f"{len(matrix)} {num_machines}\n")
        lines = []
        for job in matrix:
            for machine, time in job:
                lines.append(f"{machine} {time} ")
            lines.append('\n')
        lines.pop()  # remove the last newline character
        f.writelines(lines)

if __name__ == "__main__":
    dimension = input("Enter the matrix dimension (e.g. 3x3): ")
    num_jobs, num_machines = map(int, dimension.split('x'))
    matrix = generate_matrix(num_jobs, num_machines)
    write_matrix_to_file(matrix, f"{dimension}.txt")
    print(f"Matrix saved to {dimension}.txt")