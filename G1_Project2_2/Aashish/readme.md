# CPU Scheduling Simulator (Multicore)

This project is a simple terminal-based simulator written in **C** to understand how different CPU scheduling algorithms behave in a multicore environment.

It currently supports:
- **Lottery Scheduling**
- **Rate Monotonic Scheduling (RMS)**

The simulator runs step-by-step (tick-wise), shows which task is running on each core, and calculates performance metrics like turnaround and waiting time.

---

## Project Structure

```
main.c - Handles user input and controls execution
algorithms.c - Implements scheduling logic and timeline display
algorithms.h - Contains data structures and function prototypes
Makefile - Used to build and run the project
README.md - Project documentation
```

---

## Algorithms

### 1. Lottery Scheduling

Lottery scheduling works on a **randomized approach**.

- Every task is given a number of **tickets**
- At each step, one ticket is randomly picked
- The task owning that ticket gets CPU time
- Tasks with more tickets have a higher chance of being selected
- A **time quantum of 4** is used before switching

**Parameters used:**

| Parameter     | Meaning |
|--------------|--------|
| Burst Time   | Total execution time required |
| Arrival Time | When the task enters the system |
| Tickets      | Probability weight for scheduling |

---

### 2. Rate Monotonic Scheduling (RMS)

RMS is a **priority-based preemptive algorithm** mainly used in real-time systems.

- Each task has a **fixed period**
- Priority depends on the period:
  - Smaller period → higher priority
- If a higher priority task arrives, it immediately **preempts** the current one

**Parameters used:**

| Parameter     | Meaning |
|--------------|--------|
| Burst Time   | CPU time required |
| Arrival Time | Initial arrival time |
| Period       | Frequency of execution |

---

## How to Build and Run

### Using Makefile

```bash
make 
./scheduler.sim
```

## Usage

The tasks used in the simulation are **predefined** in the program.  
So, you don’t need to enter process details manually.

When you run the program:

1. A menu is displayed:
   - Run Lottery Scheduling
   - Run Rate Monotonic Scheduling (RMS)
   - Exit

2. Based on your choice:
   - The selected scheduling algorithm is executed
   - Tasks are initialized internally before every run

3. The simulator outputs:
   - Tick-by-tick execution across CPU cores
   - Task allocation per core
   - Final performance statistics (Turnaround Time & Waiting Time)

## Sample Test Runs

### 1: Lottery Scheduling 

#### Execution Timeline

```
--- Commencing Lottery Scheduling (Quantum = 4) ---
Tick    Core 0  Core 1
---------------------------------
00      [T1]    [--]
01      [T1]    [T2]
02      [T1]    [T2]
03      [T1]    [T2]
04      [T3]    [T2]
05      [T3]    [T2]
06      [T3]    [T2]
07      [T4]    [T2]
08      [T4]    [T2]
09      [T4]    [T1]
10      [T4]    [T1]
11      [T5]    [T1]
12      [T5]    [T1]
13      [T6]    [T7]
14      [T6]    [T7]
...
135     [T12]   [--]
136     [T12]   [--]
```

---

#### Performance Statistics

```
TID     Arrival Burst   Start   Finish  Turnaround      Waiting
---     ------- -----   -----   ------  ----------      -------
T1      0       8       0       13      13              5
T2      1       25      1       52      51              26
T3      2       3       4       7       5               2
T4      5       12      7       26      21              9
T5      8       2       11      13      5               3
T6      10      40      13      112     102             62
T7      12      5       13      18      6               1
T8      15      6       30      39      24              18
T9      20      15      21      51      31              16
T10     22      4       25      29      7               3
T11     25      8       29      37      12              4
T12     30      50      43      137     107             57
T13     35      2       38      40      5               3
T14     40      9       47      68      28              19
T15     45      3       56      59      14              11
T16     50      18      51      70      20              2
T17     55      5       59      71      16              11
T18     60      7       63      74      14              7
T19     65      22      67      96      31              9
T20     70      4       71      75      5               1
```

---

#### Averages

```
Mean Turnaround Time : 25.85
Mean Waiting Time    : 13.45
```

#### Observations

- The scheduling order is **not fixed** and may change in different runs because the algorithm is randomized  
- Tasks with higher ticket counts tend to get CPU time more frequently  
- Some tasks (like T2, T6, T12) appear multiple times due to higher probability  
- Short tasks finish early if selected quickly, but may also get delayed due to randomness  
- Long-running tasks (such as T6 and T12) occupy CPU for extended periods once selected  
- Compared to RMS, waiting and turnaround times are generally higher due to lack of strict priority


### 2: Rate Monotonic Scheduling (RMS)

#### Execution Timeline

```
--- Commencing Rate Monotonic Scheduling (RMS) ---
Priority = shortest period first (preemptive)
Tick    Core 0  Core 1
---------------------------------
00      [T1]    [--]
01      [T1]    [T2]
02      [T1]    [T3]
03      [T1]    [T3]
04      [T1]    [T3]
05      [T1]    [T4]
06      [T1]    [T4]
07      [T1]    [T4]
08      [T5]    [T4]
09      [T5]    [T4]
10      [T2]    [T4]
11      [T2]    [T4]
12      [T7]    [T4]
13      [T7]    [T4]
14      [T7]    [T4]
15      [T7]    [T8]
16      [T7]    [T8]
...
138     [T12]   [--]
139     [T12]   [--]
```

---

#### Performance Statistics

```
TID     Arrival Burst   Start   Finish  Turnaround      Waiting
---     ------- -----   -----   ------  ----------      -------
T1      0       8       0       8       8               0
T2      1       25      1       69      68              43
T3      2       3       2       5       3               0
T4      5       12      5       19      14              2
T5      8       2       8       10      2               0
T6      10      40      37      109     99              59
T7      12      5       12      17      5               0
T8      15      6       15      21      6               0
T9      20      15      20      36      16              1
T10     22      4       22      26      4               0
T11     25      8       25      33      8               0
T12     30      50      90      140     110             60
T13     35      2       35      37      2               0
T14     40      9       40      49      9               0
T15     45      3       45      48      3               0
T16     50      18      50      68      18              0
T17     55      5       55      60      5               0
T18     60      7       60      67      7               0
T19     65      22      68      90      25              3
T20     70      4       70      74      4               0
```

---

#### Averages

```
Mean Turnaround Time : 20.80
Mean Waiting Time    : 8.40
```

---

#### Observations

- Tasks with shorter periods get higher priority  
- Preemption ensures faster response for high-priority tasks  
- Overall waiting and turnaround times are lower compared to Lottery Scheduling  

## Limitations

- Lottery scheduling output is non-deterministic (random ticket selection)  
- RMS does not simulate periodic task re-release (tasks run only once)  
- No support for I/O or blocked states (all tasks are CPU-bound)  

---