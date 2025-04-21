#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

bool is_page_present(const vector<int>& frames, int capacity, int page) {
    for (int i = 0; i < capacity; i++) {
        if (frames[i] == page) return true;
    }
    return false;
}

void print_frames(const vector<int>& frames, int capacity, int page, bool page_fault) {
    cout << "Reference: " << page << " | Frames: ";
    for (int i = 0; i < capacity; i++) {
        cout << frames[i] << " ";
    }
    cout << "\t| Page Fault: " << (page_fault ? "Yes" : "No") << endl;
}

typedef void (*ReplacementFunc)(vector<int>&, int, int, const vector<int>&, int);

int run_algorithm(const vector<int>& reference_string, int n, int capacity, const string& name,
                  ReplacementFunc replace_page) {
    vector<int> frames(capacity, -1);
    cout << name << " Page Replacement:\n";
    int page_faults = 0;

    for (int i = 0; i < n; i++) {
        bool page_fault = !is_page_present(frames, capacity, reference_string[i]);
        if (page_fault) {
            page_faults++;
            replace_page(frames, capacity, reference_string[i], reference_string, i);
        }
        print_frames(frames, capacity, reference_string[i], page_fault);
    }

    return page_faults;
}

void replace_page_fcfs(vector<int>& frames, int capacity, int page, const vector<int>& reference_string, int index) {
    static int frame_index = 0;
    frames[frame_index] = page;
    frame_index = (frame_index + 1) % capacity;
}

void replace_page_lru(vector<int>& frames, int capacity, int page, const vector<int>& reference_string, int index) {
    static vector<int> time(10, 0);
    static int current_time = 0;

    for (int i = 0; i < capacity; i++) {
        if (frames[i] == -1) {
            frames[i] = page;
            time[i] = ++current_time;
            return;
        }
    }

    int lru_index = 0;
    for (int i = 1; i < capacity; i++) {
        if (time[i] < time[lru_index]) lru_index = i;
    }

    frames[lru_index] = page;
    time[lru_index] = ++current_time;
}

void replace_page_optimal(vector<int>& frames, int capacity, int page, const vector<int>& reference_string, int current_index) {
    for (int i = 0; i < capacity; i++) {
        if (frames[i] == -1) {
            frames[i] = page;
            return;
        }
    }

    int farthest = -1;
    int replace_index = -1;

    for (int i = 0; i < capacity; i++) {
        bool found = false;
        for (int j = current_index + 1; j < reference_string.size(); j++) {
            if (frames[i] == reference_string[j]) {
                if (j > farthest) {
                    farthest = j;
                    replace_index = i;
                }
                found = true;
                break;
            }
        }
        if (!found) {
            replace_index = i;
            break;
        }
    }

    if (replace_index == -1) replace_index = 0;
    frames[replace_index] = page;
}

int main() {
    int n, capacity;

    cout << "Enter number of pages in reference string: ";
    cin >> n;

    vector<int> reference_string(n);
    cout << "Enter the reference string (space-separated): ";
    for (int i = 0; i < n; i++) {
        cin >> reference_string[i];
    }

    cout << "Enter number of frames: ";
    cin >> capacity;

    int fcfs_faults = run_algorithm(reference_string, n, capacity, "FCFS", replace_page_fcfs);
    cout << "\nFCFS Total Page Faults: " << fcfs_faults << endl;

    int lru_faults = run_algorithm(reference_string, n, capacity, "LRU", replace_page_lru);
    cout << "\nLRU Total Page Faults: " << lru_faults << endl;

    int optimal_faults = run_algorithm(reference_string, n, capacity, "Optimal", replace_page_optimal);
    cout << "\nOptimal Total Page Faults: " << optimal_faults << endl;

    return 0;
}