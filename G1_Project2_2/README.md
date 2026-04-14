# G1 Project 2 — Multiprocessor CPU Scheduling Simulator

A collection of CPU scheduling algorithm simulators implemented in **C**, demonstrating how different scheduling strategies behave in a **multicore environment**. Each team member has implemented two distinct scheduling algorithms, all featuring tick-by-tick Gantt chart output, I/O handling (where applicable), and performance metrics.

---

## Team Members & Algorithms

| Member     | Algorithm 1                       | Algorithm 2                              |
|------------|-----------------------------------|------------------------------------------|
| **Aashish** | Lottery Scheduling               | Rate Monotonic Scheduling (RMS)          |
| **Abhinav** | Round Robin (RR)                 | Multilevel Feedback Queue (MLFQ)         |
| **Aditya**  | Multilevel Feedback Queue (MLFQ) | Completely Fair Scheduler (CFS)          |
| **Nagamani**| Lottery Scheduling               | MLFQ                                     |

---

## Project Structure

```
G1_Project2_2/
├── Aashish/
│   ├── Makefile
│   ├── readme.md
│   └── src/
│       ├── main.c          — Menu-driven entry point
│       ├── algorithms.c    — Lottery & RMS implementations
│       ├── algorithms.h    — Function prototypes
│       └── types.h         — Task & Core data structures
│
├── Abhinav/
│   ├── Makefile
│   ├── readme.md
│   └── src/
│       ├── main.c          — Menu-driven entry point
│       ├── algorithms.c    — Round Robin & MLFQ implementations
│       ├── algorithms.h    — Function prototypes
│       └── types.h         — Process & CPU data structures
│
├── Aditya/
│   ├── Makefile
│   └── src/
│       ├── main.c          — Menu-driven entry point
│       ├── algorithms.c    — MLFQ & CFS implementations
│       ├── algorithms.h    — Function prototypes
│       └── types.h         — Task & Core data structures (with IO + CFS fields)
│
├── nagamani/
│   ├── README.md
│   ├── main.c
│   ├── lottery.c
│   └── mlfq.c
│
├── MLFQ/                   — Standalone MLFQ reference implementation
│   ├── Makefile
│   ├── mlfq.c
│   └── types.h
│
└── README.md               — This file
```

---

## Algorithms Documentation

---

### 1. Round Robin (RR) — *Abhinav*

Round Robin is one of the simplest preemptive scheduling algorithms, designed for **time-sharing systems**.

#### How It Works

- All processes are placed in a **single FIFO ready queue**
- Each process receives a **fixed time quantum** (4 ticks in our implementation)
- When a process's quantum expires, it is moved to the **back of the queue**
- The next process in the queue gets assigned to the CPU
- If a process finishes before its quantum, the CPU is freed immediately

#### Key Characteristics

| Property               | Value                            |
|------------------------|----------------------------------|
| Preemptive             | Yes (quantum-based)              |
| Time Quantum           | 4 ticks                          |
| Starvation             | None — every process gets a turn  |
| Context Switch Overhead| High (frequent switches)         |
| Best For               | Time-sharing, interactive systems|

#### Implementation Details

- **Multicore support**: Processes are assigned to idle CPUs in order
- **No priority** — purely cyclic order
- **Metrics tracked**: Turnaround Time, Waiting Time, Response Time

#### Performance Analysis

```
Avg Turnaround Time : 36.05
Avg Waiting Time    : 23.65
Avg Response Time   : 23.65
```

**Observations**:
- Ensures fair CPU distribution among all processes
- Performance degrades with a large number of processes (higher waiting times)
- Long-running processes experience significant delays due to frequent preemption
- Quantum size is critical: too small → excessive switching, too large → degrades to FCFS

---

### 2. Lottery Scheduling — *Aashish*

Lottery Scheduling is a **probabilistic scheduling algorithm** that uses randomized ticket allocation to determine CPU access.

#### How It Works

- Each task is assigned a number of **lottery tickets**
- At each scheduling decision, a **random ticket is drawn** from the pool of ready tasks
- The task holding that ticket **wins the CPU**
- Tasks with more tickets have a **proportionally higher chance** of being selected
- A time quantum of **4 ticks** is used before re-drawing

#### Key Characteristics

| Property               | Value                                      |
|------------------------|--------------------------------------------|
| Preemptive             | Yes (quantum-based)                        |
| Time Quantum           | 4 ticks                                    |
| Starvation             | Possible but unlikely (probabilistic)      |
| Fairness               | Proportional — more tickets = more CPU     |
| Best For               | Systems needing proportional share control |

#### Implementation Details

- **Ticket-based selection**: Sum all tickets, pick a random number, iterate to find the winner
- **Multicore**: Each idle core independently draws a winner from the ready pool
- **Metrics tracked**: Turnaround Time, Waiting Time

#### Performance Analysis

```
Mean Turnaround Time : 25.85
Mean Waiting Time    : 13.45
```

**Observations**:
- Output is **non-deterministic** — different runs may produce different schedules
- Tasks with more tickets statistically receive more CPU time
- Short tasks may get delayed if unlucky in the draw
- Long-running tasks with high ticket counts dominate CPU access

---

### 3. Rate Monotonic Scheduling (RMS) — *Aashish*

RMS is a **fixed-priority preemptive algorithm** used primarily in **real-time systems**.

#### How It Works

- Each task has a fixed **period** (interval)
- Priority is determined by period: **shorter period = higher priority**
- Higher-priority tasks **preempt** lower-priority tasks immediately
- Tasks run until completion or preemption

#### Key Characteristics

| Property               | Value                                     |
|------------------------|-------------------------------------------|
| Preemptive             | Yes (priority-based preemption)           |
| Priority Assignment    | Inversely proportional to period          |
| Starvation             | Possible for low-priority (long-period)   |
| Optimal For            | Harmonic periodic task sets               |
| Best For               | Hard real-time systems                    |

#### Implementation Details

- **Sorted ready queue**: Tasks are sorted by period at every tick using insertion sort
- **Preemption**: A running task is displaced if a ready task has a shorter period
- **Multicore**: Preemption checks target the core running the lowest-priority task

#### Performance Analysis

```
Mean Turnaround Time : 20.80
Mean Waiting Time    : 8.40
```

**Observations**:
- Significantly lower turnaround and waiting times compared to Lottery Scheduling
- Deterministic — same input always produces same output
- Tasks with shorter periods dominate the CPU
- Long-period tasks may experience starvation

---

### 4. Multilevel Feedback Queue (MLFQ) — *Aditya & Abhinav*

MLFQ is an **adaptive scheduling algorithm** that automatically adjusts process priority based on behavior.

#### How It Works

- The system maintains **three priority queues**:
  - **Q0 (High)**: Time quantum = 2 ticks (Aditya) / 4 ticks (Abhinav)
  - **Q1 (Medium)**: Time quantum = 4 ticks (Aditya) / 8 ticks (Abhinav)
  - **Q2 (Low)**: Time quantum = 8 ticks (Aditya) / FCFS (Abhinav)
- All new tasks start in **Q0** (highest priority)
- If a task **exhausts its quantum**, it is **demoted** to the next lower queue
- Higher-priority queues are served first (strict priority)
- **Aditya's implementation** also includes:
  - IO burst handling (tasks block for IO and are promoted back to Q0 on completion)
  - Separate IO wait state tracking

#### Key Characteristics

| Property               | Value                                     |
|------------------------|-------------------------------------------|
| Preemptive             | Yes (quantum-based + strict priority)     |
| Queue Levels           | 3 (Q0, Q1, Q2)                            |
| Starvation             | Possible for long-running CPU-bound tasks |
| Adaptation             | Automatic demotion based on CPU usage     |
| Best For               | General-purpose systems, mixed workloads  |

#### Implementation Details (Aditya's version)

- **IO Handling**: Tasks trigger IO waits at configurable frequencies. During IO, a task enters `STATUS_WAIT` and decrements `io_rem` each tick. Upon IO completion, the task is **promoted back to Q0** for responsive scheduling.
- **Three queue levels** with strict priority: Q0 tasks always run before Q1, Q1 before Q2.
- **Multicore**: Multiple idle cores can be assigned tasks from different queue levels simultaneously.
- **15 tasks** with varying burst times, IO frequencies, and IO durations.

#### Performance Analysis (Aditya's version with IO)

```
Mean Turnaround Time : 35.73
Mean Waiting Time    : 25.00
```

#### Performance Analysis (Abhinav's version without IO)

```
Avg Turnaround Time : 32.00
Avg Waiting Time    : 19.60
Avg Response Time   : 1.25
```

**Observations**:
- Short tasks finish quickly in Q0 (excellent response time)
- Long CPU-bound tasks gradually sink to Q2 and experience higher waiting times
- IO-bound tasks benefit from automatic promotion back to Q0
- Without aging, Q2 tasks may get starved if Q0/Q1 always have ready tasks

---

### 5. Completely Fair Scheduler (CFS) — *Aditya*

CFS is inspired by the **Linux kernel's Completely Fair Scheduler**. It aims to give every task a **proportionally fair share** of CPU time using virtual runtime tracking.

#### How It Works

- Each task maintains a **virtual runtime (vruntime)** counter
- The task with the **smallest vruntime** is always scheduled next
- vruntime increments are **inversely proportional to task weight**:
  ```
  vruntime_increment = (1024 / weight) × time_executed
  ```
- Task weight is derived from a **nice value** (-5 to +5):
  - Lower nice → higher weight → slower vruntime growth → more CPU time
  - Higher nice → lower weight → faster vruntime growth → less CPU time
- **Dynamic time slices** are computed proportional to each task's weight relative to total ready weight
- **Sleeper fairness**: Tasks returning from IO have their vruntime set to the minimum vruntime of all ready tasks, preventing starvation of IO-bound processes
- **Preemption**: If a ready task's vruntime is significantly lower than a running task's, it preempts

#### Weight Table (from Linux `sched_prio_to_weight`)

| Nice Value | Weight | CPU Share (relative) |
|------------|--------|----------------------|
| -5         | 3277   | ~3.2x                |
| -4         | 2621   | ~2.6x                |
| -3         | 2097   | ~2.0x                |
| -2         | 1678   | ~1.6x                |
| -1         | 1342   | ~1.3x                |
|  0         | 1024   | 1.0x (default)       |
| +1         |  820   | ~0.8x                |
| +2         |  655   | ~0.6x                |
| +3         |  526   | ~0.5x                |
| +4         |  423   | ~0.4x                |
| +5         |  335   | ~0.3x                |

#### Key Characteristics

| Property               | Value                                          |
|------------------------|------------------------------------------------|
| Preemptive             | Yes (vruntime-based preemption)                |
| Fairness               | Proportional — weighted by nice value          |
| Starvation             | Prevented by vruntime convergence              |
| Time Slice             | Dynamic — proportional to weight               |
| IO Handling            | Sleeper fairness (vruntime reset on IO return) |
| Best For               | General-purpose, desktop, server workloads     |

#### Implementation Details

- **vruntime tracking**: Each task's vruntime grows at `1024/weight` per tick. High-weight (low-nice) tasks accumulate vruntime slower, naturally receiving more CPU time.
- **Dynamic time slices**: Computed as `(task_weight / total_ready_weight) × target_latency`, with a minimum granularity of 1 tick and a base target latency of 6 ticks.
- **Preemption**: If any ready task's vruntime + 2.0 < running task's vruntime, preemption occurs.
- **Sleeper fairness**: When a task completes IO, its vruntime is set to `min_vruntime` of all ready tasks, giving it immediate access to the CPU.
- **IO handling**: Identical to MLFQ — tasks trigger IO at configurable frequencies and durations.

#### Performance Analysis

```
Mean Turnaround Time : 38.33
Mean Waiting Time    : 27.60
```

**Observations**:
- CFS distributes CPU time proportionally — tasks with nice=-5 (T5) get ~3.2× more execution than nice=+5 (T9)
- The vruntime mechanism naturally prevents starvation, unlike priority-based systems
- IO-bound tasks benefit from sleeper fairness and get responsive scheduling after IO completion
- Higher average waiting times compared to MLFQ because CFS focuses on **fairness** rather than **throughput**
- Preemption ensures that newly arrived or IO-returning tasks are scheduled promptly
- The scheduler is fully deterministic (same workload → same schedule)

---

## How to Build and Run

Each member's implementation can be built and run independently:

### Aashish (Lottery + RMS)
```bash
cd Aashish
make
./scheduler_sim
```

### Abhinav (Round Robin + MLFQ)
```bash
cd Abhinav
make
./scheduler_sim
```

### Aditya (MLFQ + CFS)
```bash
cd Aditya
make
./scheduler_sim
```

### Nagamani
```bash
cd nagamani
# Refer to nagamani/README.md for build instructions
```

---

## Common Configuration

All implementations simulate scheduling with:
- **2 CPU cores** (multiprocessor)
- **15–20 predefined tasks** with varying arrival times and burst times
- **Tick-by-tick Gantt chart output** showing core assignments
- **Performance metrics**: Turnaround Time, Waiting Time (some include Response Time)

---

## Comparison of Algorithms

| Algorithm | Type          | Fairness            | Starvation | IO Handling | Deterministic |
|-----------|---------------|---------------------|------------|-------------|---------------|
| Round Robin | Time-sharing | Equal (cyclic)     | No         | No          | Yes           |
| Lottery   | Probabilistic | Proportional (tickets) | Unlikely | No          | No            |
| RMS       | Real-time     | Priority-based      | Possible   | No          | Yes           |
| MLFQ      | Adaptive      | Priority + feedback  | Possible   | Yes*        | Yes           |
| CFS       | Fair-share    | Proportional (weight)| No         | Yes         | Yes           |

*IO handling is present in Aditya's MLFQ implementation

---

## Limitations

- All simulators use predefined tasks (no user input for process creation)
- RMS does not simulate periodic task re-release (tasks run once)
- MLFQ does not implement aging (possible starvation in lower queues)
- CFS preemption threshold is fixed (not adaptive)
- No inter-process communication or synchronization primitives
- Context switch overhead is not modeled

---
