# CPU Scheduler Simulator

A discrete-event simulation tool written in **C** that models CPU scheduling algorithms.  
This project visualizes process execution flows and calculates performance metrics for different scheduling strategies, specifically **Round Robin** and **Preemptive Priority Scheduling**.

![C](https://img.shields.io/badge/language-C-blue.svg)
![Build](https://img.shields.io/badge/build-gcc-green.svg)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS-lightgrey.svg)

---
<img width="617" height="405" alt="image" src="https://github.com/user-attachments/assets/b222cb1f-d048-449a-83cf-89f8aca42f2a" />

## Table of Contents
- [Overview](#overview)
- [System Architecture](#system-architecture)
- [Key Features](#key-features)
- [Build Instructions](#build-instructions)
- [Usage](#usage)
- [Examples](#examples)
- [Input File Formats](#input-file-formats)
- [Output Sample](#output-sample)
- [Performance Analysis](#performance-analysis)

---

## Overview
This simulator is designed to analyze how different **Time Quantums** and **Priority levels** impact system performance.  
It features a custom-built memory management system using linked lists for process queues and dynamic arrays for process storage.

The program utilizes an **adaptive input parser** that automatically detects the required scheduling algorithm based on the structure of the input dataset.

---

## System Architecture
- **Input Parser** → Detects algorithm type (Round Robin vs Priority)
- **Scheduler Core** → Implements RR and Preemptive Priority logic
- **Data Structures** → Linked Lists for queues, dynamic arrays for storage
- **Metrics Engine** → Calculates turnaround, waiting, response times, utilization, throughput

---

## Key Features
- **Dual-Mode Scheduling**
  - **Round Robin (RR):** Standard time-slice scheduling for equal-priority tasks.
  - **Multilevel Queue with Preemption:** Priority-based scheduling where higher-priority tasks preempt running tasks, with Round Robin applied within specific priority levels.

- **Adaptive CSV Parsing**
  - Automatically detects if the input file contains priority data and switches algorithms dynamically.

- **Custom Data Structures**
  - Implements raw Linked Lists (Node/Queue) and dynamic structs from scratch in C without reliance on high-level container libraries.

- **Detailed Metrics**
  - Generates comprehensive reports including:
    - Turnaround Time
    - Waiting Time
    - Response Time
    - CPU Utilization
    - Throughput

---

## Build Instructions
The project has no external dependencies beyond the standard C library.

Clone the repository:
```bash
git clone https://github.com/yourusername/cpu_scheduler.git
cd cpu_scheduler

gcc scheduler.c -o scheduler
./scheduler <input_file.csv> <time_quantum>
```
For example:

```bash
Round Robin simulation:
./scheduler data/standard.csv 2
Priority-based simulation:
./scheduler data/priority.csv 5

```
## Input File Formats

The simulator parses standard CSV files. The logic adapts based on the number of columns detected.

---

### Mode 1: Standard Round Robin
**Columns:** PID, Arrival Time, Burst Time  
If 3 columns are detected, the system defaults to a flat Round Robin algorithm (all processes treated as equal priority).

| PID | Arrival Time | Burst Time |
|-----|--------------|------------|
| 1   | 0            | 10         |
| 2   | 2            | 5          |
| 3   | 4            | 8          |

---

### Mode 2: Preemptive Priority
**Columns:** PID, Arrival Time, Burst Time, Priority  
If 4 columns are detected, the system activates the Multilevel Queue logic.

- **Priority Scale:** 0 (Highest) to 9 (Lowest)  
- **Behavior:** A running process will be preempted immediately if a process with a higher priority (lower number) enters the queue.

| PID | Arrival Time | Burst Time | Priority |
|-----|--------------|------------|----------|
| 1   | 0            | 10         | 2        |
| 2   | 5            | 4          | 0        |
| 3   | 6            | 2          | 1        |

## Output Sample

The simulation outputs a step-by-step execution log (optional) and a final performance table:

| PID | Prio | Arrive | Burst | Finish | Turn | Wait | Resp |
|-----|------|--------|-------|--------|------|------|------|
| 2   | 0    | 1      | 4     | 9      | 8    | 4    | 4    |
| 4   | 0    | 3      | 5     | 19     | 16   | 11   | 11   |
| 1   | 0    | 0      | 8     | 22     | 22   | 14   | 0    |
| 5   | 0    | 10     | 2     | 24     | 14   | 12   | 12   |
| 3   | 0    | 2      | 9     | 28     | 26   | 17   | 7    |

---


## Output Sample

**Summary Metrics**

- **Total Simulation Time:** 28  
- **Total Context Switches:** 2  
- **Average Waiting Time:** 11.60  
- **Average Turnaround Time:** 17.20  
- **Throughput:** 0.1786 processes/unit  
- **CPU Utilization:** 92.86%  

## Performance Analysis
By varying the Time Quantum and Priority levels, users can:

- Observe trade-offs between fairness and efficiency

- Compare CPU utilization under different workloads

- Identify starvation risks in priority scheduling
  
## **The Context Switch Trade-off**
The data reveals a strict inverse relationship between the Time Quantum size and the number of Context Switches.Small Quantum (Q=1): The CPU "thrashed," performing 23 context switches. While this provides high responsiveness, it introduces significant system overhead.Large Quantum (Q=10+): The context switches dropped to 0. This indicates the system spent 100% of its time executing processes and 0% of its time managing queues.
## **Convergence to First-Come-First-Serve (FCFS)**
A critical observation is that the results for Quantum 10 and Quantum 20 are identical.Reason: The largest burst time in the input set was 9 units.Conclusion: In Round Robin, when $Time Quantum \ge Max(Burst Time)$, the algorithm degrades into FCFS. The preemption logic is never triggered because every process finishes before its time expires. Increasing the quantum beyond 10 yields no change in scheduling order.
## **Waiting Time Optimization**
For this specific dataset, larger time quantums resulted in lower Average Waiting Times (10.20 vs 12.40).Analysis: Smaller quantums (Q=1, Q=2) kept processes in the system longer by constantly pausing them to give others a turn. Larger quantums allowed short jobs (like PID 2 and 5) to exit the system immediately upon execution, removing their wait times from the average.
