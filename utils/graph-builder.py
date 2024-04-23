import matplotlib.pyplot as plt

def read_data(filename):
    try:
        with open(filename) as f:
            lines = f.readlines()
    except FileNotFoundError:
        print(f"File {filename} not found")
        return None
    except PermissionError:
        print(f"Permission denied for file {filename}")
        return None
    except:
        print(f"Error reading file {filename}")
        return None
    jobs = []
    for line in lines:
        match = re.search(r"Job (\d+) \(Machine (\d+)\): Start time = (\d+), End time = (\d+)", line)
        if match:
            job = int(match.group(1))
            machine = int(match.group(2))
            start = int(match.group(3))
            end = int(match.group(4))
            jobs.append((job, machine, start, end))
    return jobs

def plot_schedule(jobs):
    jobs.sort(key=lambda x: x[2])
    start = jobs[0][2]
    end = jobs[-1][3]
    machines = {}
    for job in jobs:
        if job[1] not in machines:
            machines[job[1]] = []
        machines[job[1]].append(job)
           
    _, ax = plt.subplots()

    ax.set_xlabel("Time")
    colors = ['red', 'blue', 'green']
    labels_shown = set()
    for i, machine in enumerate(sorted(machines.keys())):
        for job in machines[machine]:
            if job[0] not in labels_shown:
                ax.plot([job[2], job[3]], [machine, machine], label=f"Job {job[0]}", color=colors[job[0]])
                labels_shown.add(job[0])
            ax.fill_betweenx([machine-0.1, machine+0.1], job[2], job[3], color=colors[job[0]], alpha=0.3)
    ax.set_yticks(sorted(machines.keys()))
    ax.xaxis.grid(True)
    ax.set_xticks(range(start, end+1))
    ax.set_ylabel("Machine")
    ax.set_title("Jobshop Schedule")
    ax.legend()
    plt.show()
    

if __name__ == "__main__":
    import sys
    import re
    if len(sys.argv) != 2:
        print("Usage: ./schedule.py <file>")
        sys.exit(1)
    filename = sys.argv[1]
    jobs = read_data(filename)
    if jobs:
        plot_schedule(jobs)
