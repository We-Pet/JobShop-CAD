# Job-Shop Scheduling Sequential and Parallel implementations

This project implements a job scheduling algorithm using multithreading in **C/C++** to minimize the makespan for a set of jobs on multiple machines. The algorithm ensures that jobs are processed efficiently by using locks to avoid race conditions and guarantees thread-safe access to shared resources.

## Table of contents


1. [Overview](#overview)
2. [Key Concepts](#keyConcepts)
3. [Features](#features)
4. [Getting Started](#getting-started)
5. [Building the Project](#building-the-project)

## Overview

The objective of this project is to implement a parallel job scheduling algorithm that minimizes the makespan of a set of jobs on various machines. The program supports the execution of jobs using multithreading, effectively managing shared resources with locks to avoid race conditions.

## Key Concepts

1. **Makespan**: The total time required to complete all jobs.
2. **Multithreading**: The project uses threads to execute multiple jobs simultaneously, optimizing the scheduling and reducing the makespan.
3. **Locks**: Mutex locks are used to protect shared resources like machine states and makespan values.

## Features

- Handles various scenarios based on the number of threads:
    - Threads fewer than jobs
    - Threads equal to jobs
    - Threads more than jobs
- Implements a parallel version of job scheduling with thread synchronization to avoid race conditions.
- Provides results that can vary depending on the thread execution order due to concurrency.

## Getting Started

Follow these instructions to set up and run the project on your local machine.

### Prerequisites

- C/C++ compiler (e.g., GCC)
- POSIX threads (pthreads) library
- Make (optional, if using a Makefile)

## Building the Project

### Building Sequential

``make runS-%``

### Building Parallel

``make runP-%``

## Execution Modes

The program can handle different thread-to-job ratios:
- **Threads < Jobs**: Reuses threads iteratively to complete all jobs.
- **Threads = Jobs**: Each job is handled by a dedicated thread.
- **Threads > Jobs**: Threads are limited to the number of jobs available.

## Results

The sequential implementation provides consistent results due to the deterministic order of job execution. However, the parallel implementation's results may vary depending on which thread acquires the lock first.

## Sample Results:
- Sequential Implementation:
    - Input file: **ft03.jss**
    - Makespan: 20
- Parallel Implementation:
    - Input file: **ft03.jss**
    - Makespan: 11

The parallel implementation achieves different makespans because of the non-deterministic nature of thread scheduling.


