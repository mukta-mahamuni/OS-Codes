#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
using namespace std;

sem_t mutex, writeblock, printlock;
int data = 0, readerCount = 0;
int numReaders = 0, numWriters = 0;
bool isAsync = false;

void safePrint(const string& message) {
    sem_wait(&printlock);
    cout << message << flush;
    sem_post(&printlock);
}

void* reader(void* arg) {
    int id = *((int*)arg);
    safePrint("Reader " + to_string(id) + ": wanting to read\n");

    if (!isAsync) {
        sem_wait(&mutex);
        readerCount++;
        if (readerCount == 1)
            sem_wait(&writeblock);
        sem_post(&mutex);
    }

    safePrint("Reader " + to_string(id) + ": reading\n");
    sleep(1);
    safePrint("Reader " + to_string(id) + ": leaving reading\n");

    if (!isAsync) {
        sem_wait(&mutex);
        readerCount--;
        if (readerCount == 0)
            sem_post(&writeblock);
        sem_post(&mutex);
    }
    return nullptr;
}

void* writer(void* arg) {
    int id = *((int*)arg);
    safePrint("Writer " + to_string(id) + ": wanting to write\n");

    if (!isAsync)
        sem_wait(&writeblock);

    safePrint("Writer " + to_string(id) + ": writing\n");
    sleep(1);
    safePrint("Writer " + to_string(id) + ": finished\n");
    safePrint("Writer " + to_string(id) + ": leaving writing\n");

    if (!isAsync)
        sem_post(&writeblock);

    return nullptr;
}

int main() {
    pthread_t r[50], w[50];
    int rID[50], wID[50];

    cout << "Choose mode: 1 for Synchronous, 2 for Asynchronous: ";
    int mode;
    cin >> mode;
    isAsync = (mode == 2);

    cout << "Number of readers (max 50): ";
    cin >> numReaders;
    cout << "Number of writers (max 50): ";
    cin >> numWriters;

    sem_init(&mutex, 0, 1);
    sem_init(&writeblock, 0, 1);
    sem_init(&printlock, 0, 1);  // used only for clean printing

    for (int i = 0; i < numReaders; i++) {
        rID[i] = i + 1;
        pthread_create(&r[i], NULL, reader, &rID[i]);
        if (isAsync) usleep(100000);
    }

    for (int i = 0; i < numWriters; i++) {
        wID[i] = i + 1;
        pthread_create(&w[i], NULL, writer, &wID[i]);
        if (isAsync) usleep(100000);
    }

    for (int i = 0; i < numReaders; i++)
        pthread_join(r[i], NULL);
    for (int i = 0; i < numWriters; i++)
        pthread_join(w[i], NULL);

    sem_destroy(&mutex);
    sem_destroy(&writeblock);
    sem_destroy(&printlock);

    return 0;
}