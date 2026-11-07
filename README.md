# Hospital OPD / Operation Scheduling with Deadlines

**ADS Project | SDG 3 – Good Health and Well-being | C++**

## Overview
This project models a hospital's daily OPD appointments and planned operations as jobs. Each job has an ID, duration, deadline and priority/profit score. It creates schedules and compares a priority-based Greedy heuristic with First Come First Served (FCFS) and Earliest Deadline First (EDF).

## Repository contents
- `CODE.cpp` – C++ implementation and algorithm comparison
- `DOCUMENTATION.docx` – project report, design, analysis and health-equity discussion
- `PPT FILE.pptx` – project presentation

## Requirements
- C++ compiler supporting C++11 or later
- Terminal / command prompt

## Compile and run
```bash
g++ CODE.cpp -o hospital_scheduler
./hospital_scheduler
```
On Windows:
```bash
g++ CODE.cpp -o hospital_scheduler.exe
hospital_scheduler.exe
```

## Input format
1. Available working time in minutes
2. Number of jobs
3. For each job: `JobID Duration Deadline Priority`

All time values are minutes from the beginning of the scheduling period. A larger priority value means a higher administrative priority in this demonstration.

## Example
```text
Enter available working time in minutes: 480
Enter number of jobs/patients: 5
J1 120 180 90
J2 60 120 80
J3 90 300 70
J4 180 360 95
J5 60 240 60
```

The program displays each algorithm's schedule, missed/rejected jobs, time used and utilization.

## Algorithms
- **Greedy:** Sorts by descending priority; ties use earlier deadline and shorter duration. It tries feasible insertion positions and accepts a job only if all scheduled jobs still meet their deadlines and fit the shift.
- **FCFS:** Preserves the input/arrival order.
- **EDF:** Sorts by increasing deadline.

For FCFS and EDF, jobs that exceed shift capacity are not scheduled. Jobs that finish after their deadlines are shown as late/missed.

## Complexity
Let `n` be the number of jobs.
- Greedy sorting: `O(n log n)`.
- Greedy insertion/feasibility checks: worst-case `O(n^3)` in this implementation.
- FCFS: `O(n)` after input.
- EDF sorting and scheduling: `O(n log n)`.

## Important limitation
The Greedy method is a practical priority-based heuristic for variable-duration jobs. It is **not guaranteed to maximize total priority/profit globally**. The classic job-sequencing-with-deadlines greedy algorithm usually assumes unit-time jobs; variable-duration scheduling is a different, harder problem.

## Health-equity and safety
This is an educational scheduling model, not a clinical decision system. Real hospitals must:
- Give emergency and clinically urgent patients precedence based on qualified clinical triage.
- Never use financial profit as a substitute for medical need.
- Reserve capacity for emergencies and protect access for vulnerable patients.
- Avoid discrimination based on income, gender, disability, age, caste, religion or location.
- Include clinician judgment, consent, infection-control needs, equipment and staff availability.
- Monitor waiting times and missed appointments across patient groups.

## SDG 3 connection
The project supports discussion of timely access to essential healthcare and efficient use of limited hospital resources, contributing to **SDG 3: Good Health and Well-being**.

## Team details
Add your team members, roll numbers, college and faculty guide before submission.
