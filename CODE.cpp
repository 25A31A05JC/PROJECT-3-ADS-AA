#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
using namespace std;

struct Job {
    string id;
    int duration;   // minutes
    int deadline;   // minutes from start of the day
    int priority;   // larger value means higher priority
    int arrival;    // arrival order for FCFS
};

struct Result {
    string algorithm;
    vector<pair<Job, int>> scheduled; // job and completion time
    vector<Job> missed;
    int used = 0;
    int capacity = 0;
};

bool feasibleAt(const vector<pair<Job, int>>& order, const Job& candidate,
                int capacity, int insertPos) {
    vector<Job> trial;
    for (int i = 0; i < (int)order.size() + 1; ++i) {
        if (i == insertPos) trial.push_back(candidate);
        if (i < (int)order.size()) trial.push_back(order[i].first);
    }

    int elapsed = 0;
    for (const Job& job : trial) {
        elapsed += job.duration;
        if (elapsed > job.deadline || elapsed > capacity) return false;
    }
    return true;
}

Result runGreedy(vector<Job> jobs, int capacity) {
    // Priority/profit-based greedy heuristic. Ties: earlier deadline, then shorter job.
    sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {
        if (a.priority != b.priority) return a.priority > b.priority;
        if (a.deadline != b.deadline) return a.deadline < b.deadline;
        return a.duration < b.duration;
    });

    Result r;
    r.algorithm = "Greedy (priority-based)";
    r.capacity = capacity;
    vector<Job> order;

    for (const Job& job : jobs) {
        bool placed = false;
        // Try every insertion position; accept only if all scheduled deadlines remain feasible.
        for (int pos = 0; pos <= (int)order.size(); ++pos) {
            vector<pair<Job, int>> current;
            int elapsed = 0;
            for (const Job& x : order) {
                elapsed += x.duration;
                current.push_back({x, elapsed});
            }
            if (feasibleAt(current, job, capacity, pos)) {
                order.insert(order.begin() + pos, job);
                placed = true;
                break;
            }
        }
        if (!placed) r.missed.push_back(job);
    }

    int elapsed = 0;
    for (const Job& job : order) {
        elapsed += job.duration;
        r.scheduled.push_back({job, elapsed});
    }
    r.used = elapsed;
    return r;
}

Result runBaseline(vector<Job> jobs, int capacity, string mode) {
    if (mode == "EDF") {
        stable_sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {
            return a.deadline < b.deadline;
        });
    } else {
        stable_sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {
            return a.arrival < b.arrival;
        });
    }

    Result r;
    r.algorithm = (mode == "EDF") ? "Earliest Deadline First" : "First Come First Served";
    r.capacity = capacity;
    int elapsed = 0;

    for (const Job& job : jobs) {
        if (elapsed + job.duration > capacity) {
            r.missed.push_back(job);
            continue;
        }
        elapsed += job.duration;
        r.scheduled.push_back({job, elapsed});
        if (elapsed > job.deadline) r.missed.push_back(job);
    }
    r.used = elapsed;
    return r;
}

void printResult(const Result& r) {
    cout << "\n--- " << r.algorithm << " ---\n";
    cout << left << setw(10) << "Job ID" << setw(14) << "Duration"
         << setw(14) << "Deadline" << setw(14) << "Priority"
         << "Completion\n";
    for (const auto& item : r.scheduled) {
        const Job& j = item.first;
        cout << left << setw(10) << j.id << setw(14) << j.duration
             << setw(14) << j.deadline << setw(14) << j.priority
             << item.second;
        if (item.second > j.deadline) cout << "  (LATE)";
        cout << '\n';
    }

    cout << "Missed / rejected jobs: ";
    if (r.missed.empty()) cout << "None";
    else for (const Job& j : r.missed) cout << j.id << " ";
    cout << "\nScheduled jobs: " << r.scheduled.size()
         << "\nTime used: " << r.used << " / " << r.capacity << " minutes"
         << "\nUtilization: " << fixed << setprecision(2)
         << (r.capacity ? (100.0 * r.used / r.capacity) : 0.0) << "%\n";
}

int main() {
    int n, capacity;
    cout << "HOSPITAL OPD / OPERATION SCHEDULER\n";
    cout << "Enter available working time in minutes: ";
    cin >> capacity;
    cout << "Enter number of jobs/patients: ";
    cin >> n;

    if (capacity <= 0 || n <= 0) {
        cout << "Please enter positive capacity and number of jobs.\n";
        return 0;
    }

    vector<Job> jobs(n);
    cout << "\nEnter each job as: ID Duration Deadline Priority\n";
    cout << "(All times are in minutes from the start of the day.)\n";
    for (int i = 0; i < n; ++i) {
        cin >> jobs[i].id >> jobs[i].duration >> jobs[i].deadline >> jobs[i].priority;
        jobs[i].arrival = i;
        if (jobs[i].duration <= 0 || jobs[i].deadline < 0) {
            cout << "Invalid duration/deadline. Please run again with valid values.\n";
            return 0;
        }
    }

    Result greedy = runGreedy(jobs, capacity);
    Result fcfs = runBaseline(jobs, capacity, "FCFS");
    Result edf = runBaseline(jobs, capacity, "EDF");

    printResult(greedy);
    printResult(fcfs);
    printResult(edf);

    cout << "\nNote: Greedy is a priority-based heuristic for variable-duration jobs,";
    cout << "\nnot a guarantee of globally optimal profit. Clinical urgency must";
    cout << "\noverride administrative priority in real hospitals.\n";
    return 0;
}
