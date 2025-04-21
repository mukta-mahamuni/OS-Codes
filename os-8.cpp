#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cmath>
using namespace std;

int abs_diff(int a, int b) {
    return abs(a - b);
}

void fcfs(const vector<int>& requests, int head) {
    int total = 0, pos = head;
    cout << "FCFS Sequence: " << pos << " ";
    for (int r : requests) {
        total += abs_diff(pos, r);
        pos = r;
        cout << r << " ";
    }
    cout << "\nTotal Movement (FCFS): " << total << "\n";
}

void sstf(vector<int> requests, int head) {
    int total = 0, pos = head;
    vector<bool> visited(requests.size(), false);
    cout << "SSTF Sequence: " << pos << " ";
    
    for (int i = 0; i < requests.size(); ++i) {
        int min_diff = INT_MAX, idx = -1;
        for (int j = 0; j < requests.size(); ++j) {
            if (!visited[j] && abs_diff(pos, requests[j]) < min_diff) {
                min_diff = abs_diff(pos, requests[j]);
                idx = j;
            }
        }
        visited[idx] = true;
        total += min_diff;
        pos = requests[idx];
        cout << pos << " ";
    }
    cout << "\nTotal Movement (SSTF): " << total << "\n";
}

void scan(vector<int> requests, int head, int disk_size, char direction) {
    int total = 0, pos = head;
    vector<int> left, right;

    for (int r : requests) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }

    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    cout << "SCAN Sequence: " << pos << " ";
    if (direction == 'l') {
        for (int i = left.size() - 1; i >= 0; --i) {
            total += abs_diff(pos, left[i]);
            pos = left[i];
            cout << pos << " ";
        }
        total += abs_diff(pos, 0);
        pos = 0;
        cout << pos << " ";
        for (int r : right) {
            total += abs_diff(pos, r);
            pos = r;
            cout << pos << " ";
        }
    } else {
        for (int r : right) {
            total += abs_diff(pos, r);
            pos = r;
            cout << pos << " ";
        }
        total += abs_diff(pos, disk_size - 1);
        pos = disk_size - 1;
        cout << pos << " ";
        for (int i = left.size() - 1; i >= 0; --i) {
            total += abs_diff(pos, left[i]);
            pos = left[i];
            cout << pos << " ";
        }
    }
    cout << "\nTotal Movement (SCAN): " << total << "\n";
}

void c_scan(vector<int> requests, int head, int disk_size, char direction) {
    int total = 0, pos = head;
    vector<int> left, right;

    for (int r : requests) {
        if (r < head) left.push_back(r);
        else right.push_back(r);
    }

    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    cout << "C-SCAN Sequence: " << pos << " ";
    if (direction == 'r') {
        for (int r : right) {
            total += abs_diff(pos, r);
            pos = r;
            cout << pos << " ";
        }
        if (pos != disk_size - 1) {
            total += abs_diff(pos, disk_size - 1);
            pos = disk_size - 1;
            cout << pos << " ";
        }
        total += abs_diff(pos, 0);
        pos = 0;
        cout << pos << " ";
        for (int r : left) {
            total += abs_diff(pos, r);
            pos = r;
            cout << pos << " ";
        }
    } else {
        for (int i = left.size() - 1; i >= 0; --i) {
            total += abs_diff(pos, left[i]);
            pos = left[i];
            cout << pos << " ";
        }
        if (pos != 0) {
            total += abs_diff(pos, 0);
            pos = 0;
            cout << pos << " ";
        }
        total += abs_diff(pos, disk_size - 1);
        pos = disk_size - 1;
        cout << pos << " ";
        for (int i = right.size() - 1; i >= 0; --i) {
            total += abs_diff(pos, right[i]);
            pos = right[i];
            cout << pos << " ";
        }
    }
    cout << "\nTotal Movement (C-SCAN): " << total << "\n";
}

int main() {
    int n, head, disk_size, choice;
    char direction;

    cout << "Enter number of disk requests: ";
    cin >> n;
    vector<int> requests(n);
    cout << "Enter disk requests:\n";
    for (int& r : requests) cin >> r;

    cout << "Enter initial head position: ";
    cin >> head;
    cout << "Enter disk size: ";
    cin >> disk_size;
    cout << "Enter direction (l/r): ";
    cin >> direction;

    do {
        cout << "\n--- Menu ---\n";
        cout << "1. FCFS\n2. SSTF\n3. SCAN\n4. C-SCAN\n5. Exit\n";
        cout << "Choice: ";
        cin >> choice;
        switch (choice) {
            case 1: fcfs(requests, head); break;
            case 2: sstf(requests, head); break;
            case 3: scan(requests, head, disk_size, direction); break;
            case 4: c_scan(requests, head, disk_size, direction); break;
            case 5: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 5);

    return 0;
}