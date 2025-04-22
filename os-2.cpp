#include <iostream>
#include <list>
#include <algorithm>
#include <climits>
using namespace std;

struct Process {
    int pid, arrival_time, burst_time, remaining_time, priority;
    int completion_time, turn_around_time, waiting_time, response_time;
    bool first_execution;

    Process() : pid(0), arrival_time(0), burst_time(0), remaining_time(0),
                priority(0), completion_time(0), turn_around_time(0),
                waiting_time(0), response_time(0), first_execution(true) {}
};

void printProcesses(const list<Process>& processes, float avg_tat, float avg_wt) {
    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT";
    for (const auto& p : processes) {
        cout << "\n" << p.pid << "\t" << p.arrival_time << "\t" << p.burst_time
             << "\t" << p.completion_time << "\t" << p.turn_around_time << "\t" << p.waiting_time;
    }
    cout << "\nAverage TAT: " << avg_tat << " | Average WT: " << avg_wt << endl;
}

void sortByArrival(list<Process>& processes) {
    processes.sort([](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
    });
}

void FCFS(list<Process>& processes) {
    int current_time = 0;
    float total_tat = 0, total_wt = 0;
    for (auto& p : processes) {
        current_time = max(current_time, p.arrival_time);
        p.completion_time = current_time + p.burst_time;
        current_time = p.completion_time;
        p.turn_around_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turn_around_time - p.burst_time;
        total_tat += p.turn_around_time;
        total_wt += p.waiting_time;
    }
    printProcesses(processes, total_tat / processes.size(), total_wt / processes.size());
}

void SJF(list<Process>& processes) {
    int current_time = 0, completed = 0, n = processes.size();
    float total_tat = 0, total_wt = 0;
    for (auto& p : processes) p.remaining_time = p.burst_time;

    while (completed < n) {
        Process* shortest = nullptr;
        for (auto& p : processes) {
            if (p.arrival_time <= current_time && p.remaining_time > 0) {
                if (!shortest || p.burst_time < shortest->burst_time)
                    shortest = &p;
            }
        }
        if (!shortest) { current_time++; continue; }

        shortest->completion_time = current_time + shortest->burst_time;
        current_time = shortest->completion_time;
        shortest->turn_around_time = shortest->completion_time - shortest->arrival_time;
        shortest->waiting_time = shortest->turn_around_time - shortest->burst_time;
        shortest->remaining_time = 0;
        completed++;
        total_tat += shortest->turn_around_time;
        total_wt += shortest->waiting_time;
    }

    printProcesses(processes, total_tat / n, total_wt / n);
}

void SRTF(list<Process>& processes) {
    int current_time = 0, completed = 0, n = processes.size();
    float total_tat = 0, total_wt = 0;
    for (auto& p : processes) p.remaining_time = p.burst_time;

    while (completed < n) {
        Process* shortest = nullptr;
        for (auto& p : processes) {
            if (p.arrival_time <= current_time && p.remaining_time > 0) {
                if (!shortest || p.remaining_time < shortest->remaining_time)
                    shortest = &p;
            }
        }
        if (!shortest) { current_time++; continue; }

        shortest->remaining_time--;
        current_time++;
        if (shortest->remaining_time == 0) {
            shortest->completion_time = current_time;
            shortest->turn_around_time = shortest->completion_time - shortest->arrival_time;
            shortest->waiting_time = shortest->turn_around_time - shortest->burst_time;
            completed++;
            total_tat += shortest->turn_around_time;
            total_wt += shortest->waiting_time;
        }
    }
    printProcesses(processes, total_tat / n, total_wt / n);
}

void Priority(list<Process>& processes, bool preemptive) {
    int current_time = 0, completed = 0, n = processes.size();
    float total_tat = 0, total_wt = 0;

    for (auto& process : processes)
        process.remaining_time = process.burst_time;

    while (completed < n) {
        Process* selected = nullptr;
        int highest_priority = -1;
        for (auto& process : processes) {
            if (process.arrival_time <= current_time &&
                process.remaining_time > 0 &&
                process.priority > highest_priority) {
                highest_priority = process.priority;
                selected = &process;
            }
        }

        if (!selected) { current_time++; continue; }

        if (preemptive) {
            selected->remaining_time--;
            current_time++;
            if (selected->remaining_time == 0) {
                selected->completion_time = current_time;
                selected->turn_around_time = selected->completion_time - selected->arrival_time;
                selected->waiting_time = selected->turn_around_time - selected->burst_time;
                completed++;
                total_tat += selected->turn_around_time;
                total_wt += selected->waiting_time;
            }
        } else {
            selected->completion_time = current_time + selected->remaining_time;
            current_time = selected->completion_time;
            selected->turn_around_time = selected->completion_time - selected->arrival_time;
            selected->waiting_time = selected->turn_around_time - selected->burst_time;
            selected->remaining_time = 0;
            completed++;
            total_tat += selected->turn_around_time;
            total_wt += selected->waiting_time;
        }
    }
    printProcesses(processes, total_tat / n, total_wt / n);
}

void RR(list<Process>& processes, int time_quantum) {
    int current_time = 0, completed = 0, n = processes.size();
    list<Process*> queue;
    float total_tat = 0, total_wt = 0;

    for (auto& p : processes) {
        p.remaining_time = p.burst_time;
        p.response_time = -1;
    }

    while (completed < n) {
        // Add new arrivals to the queue
        for (auto& p : processes) {
            if (p.arrival_time <= current_time && p.remaining_time > 0 && p.first_execution) {
                queue.push_back(&p);
                p.first_execution = false;
            }
        }

        if (queue.empty()) {
            current_time++;
            continue;
        }

        Process* current = queue.front();
        queue.pop_front();

        if (current->response_time == -1)
            current->response_time = current_time - current->arrival_time;

        int exec_time = min(time_quantum, current->remaining_time);
        current->remaining_time -= exec_time;
        current_time += exec_time;

        // Add new arrivals again after time has moved
        for (auto& p : processes) {
            if (p.arrival_time <= current_time && p.remaining_time > 0 && p.first_execution) {
                queue.push_back(&p);
                p.first_execution = false;
            }
        }

        if (current->remaining_time > 0) {
            queue.push_back(current);  // Put back if not finished
        } else {
            current->completion_time = current_time;
            current->turn_around_time = current_time - current->arrival_time;
            current->waiting_time = current->turn_around_time - current->burst_time;
            total_tat += current->turn_around_time;
            total_wt += current->waiting_time;
            completed++;
        }
    }

    printProcesses(processes, total_tat / n, total_wt / n);
}

int main() {
    int choice;
    while (true) {
        int n;
        cout << "\nEnter number of processes (or 0 to exit): ";
        cin >> n;
        if (n == 0) break;

        list<Process> processes;
        for (int i = 0; i < n; i++) {
            Process p;
            p.pid = i + 1;
            cout << "\nEnter Arrival Time and Burst Time for Process " << p.pid << ": ";
            cin >> p.arrival_time >> p.burst_time;
            processes.push_back(p);
        }

        cout << "\nSelect Scheduling Algorithm:\n"
             << "1. FCFS\n2. SJF\n3. SRTF\n4. Priority (Preemptive)\n5. Priority (Non-Preemptive)\n6. Round Robin\nChoice: ";
        cin >> choice;

        sortByArrival(processes);

        switch (choice) {
            case 1: FCFS(processes); break;
            case 2: SJF(processes); break;
            case 3: SRTF(processes); break;
            case 4:
            case 5:
                for (auto& p : processes) {
                    cout << "\nEnter Priority for Process " << p.pid << ": ";
                    cin >> p.priority;
                }
                Priority(processes, choice == 4);
                break;
            case 6: {
                int quantum;
                cout << "\nEnter Time Quantum: ";
                cin >> quantum;
                RR(processes, quantum);
                break;
            }
            default:
                cout << "Invalid choice!";
                break;
        }
    }

    return 0;
}
