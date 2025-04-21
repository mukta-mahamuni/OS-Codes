#include <iostream> 
#include <vector> 
 
using namespace std; 
 
void displaySystemState(const vector<vector<int>>& max_demand,
                        const vector<vector<int>>& allocated_resources,
                        const vector<vector<int>>& remaining_demand,
                        const vector<int>& available_resources,
                        int total_processes, int total_resources) {

    // Find safe sequence to determine resource release order
    vector<int> work = available_resources;
    vector<bool> is_completed(total_processes, false);
    vector<int> safe_sequence;
    vector<vector<int>> current_available_list;

    int completed = 0;
    while (completed < total_processes) {
        bool found = false;
        for (int i = 0; i < total_processes; ++i) {
            if (!is_completed[i]) {
                bool can_allocate = true;
                for (int j = 0; j < total_resources; ++j) {
                    if (remaining_demand[i][j] > work[j]) {
                        can_allocate = false;
                        break;
                    }
                }

                if (can_allocate) {
                    // Release resources
                    for (int j = 0; j < total_resources; ++j)
                        work[j] += allocated_resources[i][j];

                    current_available_list.push_back(work);  // Save state after release
                    is_completed[i] = true;
                    safe_sequence.push_back(i);
                    ++completed;
                    found = true;
                }
            }
        }

        if (!found) break;  // Unsafe state — no safe sequence
    }

    // Display system state
    cout << "\n==========================================================================\n";
    cout << "| Process | Max Demand | Allocated | Remaining | Curr Avail After Release |\n";
    cout << "--------------------------------------------------------------------------\n";

    for (size_t idx = 0; idx < safe_sequence.size(); ++idx) {
        int i = safe_sequence[idx];
        cout << "| P" << i << "      | ";
        for (int j = 0; j < total_resources; ++j)
            cout << max_demand[i][j] << " ";
        cout << "      | ";
        for (int j = 0; j < total_resources; ++j)
            cout << allocated_resources[i][j] << " ";
        cout << "     | ";
        for (int j = 0; j < total_resources; ++j)
            cout << remaining_demand[i][j] << " ";
        cout << "    | ";
        for (int j = 0; j < total_resources; ++j)
            cout << current_available_list[idx][j] << " ";
        cout << " |" << endl;
    }

    cout << "\nInitial Available Resources: ";
    for (int j = 0; j < total_resources; ++j)
        cout << available_resources[j] << " ";
    cout << "\n==========================================================================\n";
}

 
bool checkSafety(vector<vector<int>>& remaining_demand, 
                 vector<vector<int>>& allocated_resources, 
                 vector<int>& available_resources, int total_processes, int total_resources) { 
    vector<int> work = available_resources; 
    vector<bool> is_completed(total_processes, false); 
    vector<int> safe_sequence; 
    int completed_count = 0; 
 
    while (completed_count < total_processes) { 
        bool found = false; 
        for (int i = 0; i < total_processes; ++i) { 
            if (!is_completed[i]) { 
                bool can_allocate = true; 
                for (int j = 0; j < total_resources; ++j) { 
                    if (remaining_demand[i][j] > work[j]) { 
                        can_allocate = false; 
                        break; 
                    } 
                } 
 
                if (can_allocate) { 
                    for (int j = 0; j < total_resources; ++j) { 
                        work[j] += allocated_resources[i][j]; 
                    } 
                    is_completed[i] = true; 
                    safe_sequence.push_back(i); 
                    ++completed_count; 
                    found = true; 
                } 
            } 
        } 
 
        if (!found) { 
            cout << "\n[!] System is in an unsafe state. No safe sequence exists." << endl; 
            return false; 
        } 
    } 
 
    cout << "\n[✔] System is in a safe state. Safe Sequence: "; 
    for (size_t i = 0; i < safe_sequence.size(); ++i) { 
        cout << "P" << safe_sequence[i]; 
        if (i != safe_sequence.size() - 1) { 
            cout << " -> "; 
        } 
    } 
    cout << "\n==================================================\n"; 
    return true; 
} 
 
bool processRequest(vector<vector<int>>& max_demand, 
                    vector<vector<int>>& allocated_resources, 
                    vector<vector<int>>& remaining_demand, 
                    vector<int>& available_resources, int process_id, vector<int>& request, 
                    int total_resources) { 
    for (int i = 0; i < total_resources; ++i) { 
        if (request[i] > remaining_demand[process_id][i]) { 
            cout << "\n[!] Error: P" << process_id << " requested more than its need!\n"; 
            return false; 
        } 
    } 
 
    for (int i = 0; i < total_resources; ++i) { 
        if (request[i] > available_resources[i]) { 
            cout << "\n[!] Resources not available. P" << process_id << " must wait.\n"; 
            return false; 
        } 
    } 
 
    for (int i = 0; i < total_resources; ++i) { 
        available_resources[i] -= request[i]; 
        allocated_resources[process_id][i] += request[i]; 
        remaining_demand[process_id][i] -= request[i]; 
    } 
 
    if (!checkSafety(remaining_demand, allocated_resources, available_resources, 
max_demand.size(), total_resources)) { 
        for (int i = 0; i < total_resources; ++i) { 
            available_resources[i] += request[i]; 
            allocated_resources[process_id][i] -= request[i]; 
            remaining_demand[process_id][i] += request[i]; 
        } 
        cout << "\n[!] Request denied: Leads to an unsafe state.\n"; 
        return false; 
    } 
 
    cout << "\n[✔] Request granted successfully.\n"; 
    return true; 
} 
 
int main() { 
    int total_processes, total_resources; 
 
    cout << "Enter number of processes: "; 
    cin >> total_processes; 
    cout << "Enter number of resources: "; 
    cin >> total_resources; 
 
    vector<vector<int>> max_demand(total_processes, vector<int>(total_resources)); 
    vector<vector<int>> allocated_resources(total_processes, vector<int>(total_resources)); 
    vector<vector<int>> remaining_demand(total_processes, vector<int>(total_resources)); 
    vector<int> available_resources(total_resources); 
 
    cout << "Enter Max Demand matrix:" << endl; 
    for (int i = 0; i < total_processes; ++i) 
        for (int j = 0; j < total_resources; ++j) 
            cin >> max_demand[i][j]; 
 
    cout << "Enter Allocated Resources matrix:" << endl; 
    for (int i = 0; i < total_processes; ++i) 
        for (int j = 0; j < total_resources; ++j) 
            cin >> allocated_resources[i][j]; 
 
    cout << "Enter Available Resources: "; 
    for (int i = 0; i < total_resources; ++i) 
        cin >> available_resources[i]; 
 
    for (int i = 0; i < total_processes; ++i) 
        for (int j = 0; j < total_resources; ++j) 
            remaining_demand[i][j] = max_demand[i][j] - allocated_resources[i][j]; 
 
    displaySystemState(max_demand, allocated_resources, remaining_demand, available_resources, 
total_processes, total_resources); 
 
    checkSafety(remaining_demand, allocated_resources, available_resources, total_processes, 
total_resources); 
 
    char choice; 
    do { 
        int process_id; 
        vector<int> request(total_resources); 
        cout << "\nEnter Process ID making a request: "; 
        cin >> process_id; 
        cout << "Enter requested resources: "; 
        for (int i = 0; i < total_resources; ++i) 
            cin >> request[i]; 
 
        processRequest(max_demand, allocated_resources, remaining_demand, available_resources, 
process_id, request, total_resources); 
 
        cout << "\nDo you want to request more resources? (y/n): "; 
        cin >> choice; 
    } while (choice == 'y' || choice == 'Y'); 
 
    return 0; 
}