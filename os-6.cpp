#include <iostream>
#include <vector>
using namespace std;

void printAllocation(const vector<int>& allocation, const vector<int>& processes) {
 cout << "\nProcess No.\tProcess Size\tBlock No.\n";
 for (int i = 0; i < processes.size(); i++) {
 cout << i + 1 << "\t\t" << processes[i] << "\t\t";
 if (allocation[i] != -1)
 cout << allocation[i] + 1;
 else
 cout << "Not Allocated";
 cout << endl;
 }
}

vector<int> firstFit(vector<int> blocks, const vector<int>& processes) {
 vector<int> allocation(processes.size(), -1);
 for (int i = 0; i < processes.size(); i++) {
 for (int j = 0; j < blocks.size(); j++) {
 if (blocks[j] >= processes[i]) {
 allocation[i] = j; 
 blocks[j] -= processes[i];
 break;
 }
 }       
}
 return allocation;
}

vector<int> bestFit(vector<int> blocks, const vector<int>& processes) {
 vector<int> allocation(processes.size(), -1);
 for (int i = 0; i < processes.size(); i++) {
 int bestIdx = -1;
 for (int j = 0; j < blocks.size(); j++) {
 if (blocks[j] >= processes[i]) {
 if (bestIdx == -1 || blocks[j] < blocks[bestIdx]) {
 bestIdx = j;
 }
 }
 }
 if (bestIdx != -1) {
 allocation[i] = bestIdx;
 blocks[bestIdx] -= processes[i];
 }
 }
 return allocation;
}

vector<int> worstFit(vector<int> blocks, const vector<int>& processes) {
 vector<int> allocation(processes.size(), -1);
 for (int i = 0; i < processes.size(); i++) {
 int worstIdx = -1;
 for (int j = 0; j < blocks.size(); j++) {
 if (blocks[j] >= processes[i]) {
 if (worstIdx == -1 || blocks[j] > blocks[worstIdx]) {
 worstIdx = j;
 }
 }
 }
 if (worstIdx != -1) {
 allocation[i] = worstIdx;
 blocks[worstIdx] -= processes[i];
 }
 }
 return allocation;
}

vector<int> nextFit(vector<int> blocks, const vector<int>& processes, int& lastAllocated) {
 vector<int> allocation(processes.size(), -1);
 int n = blocks.size();
 int j = lastAllocated;
 for (int i = 0; i < processes.size(); i++) {
 int count = 0;
 while (count < n) {
if (blocks[j] >= processes[i]) {
 allocation[i] = j;
 blocks[j] -= processes[i];
 lastAllocated = j;
 break;
 }
 j = (j + 1) % n;
 count++;
 }
 }
 return allocation;
}


int main() {
 int nBlocks, nProcesses, numToAllocate, lastAllocated = 0;
 cout << "Enter number of memory blocks: ";
 cin >> nBlocks;
 vector<int> blocksOriginal(nBlocks);
 cout << "Enter sizes of memory blocks:\n";
 for (int i = 0; i < nBlocks; i++) {
 cin >> blocksOriginal[i];
 }
 cout << "Enter number of processes: ";
 cin >> nProcesses;
 vector<int> processes(nProcesses);
 cout << "Enter sizes of processes:\n";
 for (int i = 0; i < nProcesses; i++) {
 cin >> processes[i];
 }
cout << "Enter number of processes to allocate (<= " << nProcesses << "): ";
 cin >> numToAllocate;
 vector<int> processesToAllocate(processes.begin(), processes.begin() + numToAllocate);
 int choice;
 do {
 cout << "\n=== Memory Allocation Menu ===\n";
 cout << "1. First Fit\n";
 cout << "2. Best Fit\n";
 cout << "3. Worst Fit\n";
 cout << "4. Next Fit\n";
 cout << "5. Exit\n";
 cout << "Enter your choice: ";
 cin >> choice;
 vector<int> allocation;
 vector<int> blocks = blocksOriginal; // Reset blocks for each strategy
 switch (choice) {
 case 1:
 allocation = firstFit(blocks, processesToAllocate);
 printAllocation(allocation, processesToAllocate);
 break;
 case 2:
 allocation = bestFit(blocks, processesToAllocate);
 printAllocation(allocation, processesToAllocate);
 break;
 case 3:
 allocation = worstFit(blocks, processesToAllocate);
printAllocation(allocation, processesToAllocate);
 break;
 case 4:
 cout << "Enter last allocated block index (0-based): ";
 cin >> lastAllocated;
 allocation = nextFit(blocks, processesToAllocate, lastAllocated);
 printAllocation(allocation, processesToAllocate);
 break;
 case 5:
 cout << "Exiting program.\n";
 break;
 
 default:
 cout << "Invalid choice. Try again.\n";
 }
 } while (choice != 5);
 return 0;
}