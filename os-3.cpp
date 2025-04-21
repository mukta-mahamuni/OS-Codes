#include <iostream>
#include <queue>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

using namespace std;

void named_pipe_bidirectional() {
    const char* fifo1 = "/tmp/fifo1";
    const char* fifo2 = "/tmp/fifo2";
    mkfifo(fifo1, 0666);
    mkfifo(fifo2, 0666);

    queue<string> messageQueue; // Create a message queue

    pid_t pid = fork();
    if (pid > 0) {
        // Parent process
        // Open fifo1 in write-only mode to send a message
        int fd1 = open(fifo1, O_WRONLY);

        // Push message into the queue
        string message = "Hello from Parent!";
        messageQueue.push(message);

        // Now, send the message from the queue
        write(fd1, messageQueue.front().c_str(), messageQueue.front().size() + 1);
        cout << "Parent sent: " << messageQueue.front() << endl;
        messageQueue.pop(); // Remove the message from the queue after it's sent

        close(fd1);

        // Parent reads from fifo2
        int fd2 = open(fifo2, O_RDONLY);
        char buffer[1024];
        read(fd2, buffer, sizeof(buffer));
        cout << "Parent received: " << buffer << endl;
        close(fd2);

        wait(NULL);
        unlink(fifo1);
        unlink(fifo2);
    } else {
        // Child process
        // Child reads from fifo1
        int fd1 = open(fifo1, O_RDONLY);
        char buffer[1024];
        read(fd1, buffer, sizeof(buffer));
        cout << "Child received: " << buffer << endl;
        close(fd1);

        // Child sends a reply to fifo2
        int fd2 = open(fifo2, O_WRONLY);
        string reply = "Hello from Child!";
        write(fd2, reply.c_str(), reply.size() + 1);
        cout << "Child sent: " << reply << endl;
        close(fd2);
        _exit(0);
    }
}

void simulate_redirection() {
    int pipefds[2];
    pipe(pipefds);
    pid_t pid1 = fork();

    if (pid1 == 0) {
        // First child redirects stdout to pipe
        close(pipefds[0]);
        dup2(pipefds[1], STDOUT_FILENO);
        close(pipefds[1]);
        cout << "Hello from Parent process redirected!" << endl;
        _exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Second child reads from pipe and stores in queue
        close(pipefds[1]);
        char ch;
        queue<char> bufferQueue;
        while (read(pipefds[0], &ch, 1) > 0) {
            bufferQueue.push(ch);
        }
        cout << "Child received redirected output: ";
        while (!bufferQueue.empty()) {
            cout << bufferQueue.front();
            bufferQueue.pop();
        }
        cout << endl;
        close(pipefds[0]);
        _exit(0);
    }

    // Parent process
    close(pipefds[0]);
    close(pipefds[1]);
    wait(NULL);
    wait(NULL);
}

int main() {
    cout << "=== Named Pipe Bidirectional Communication ===" << endl;
    named_pipe_bidirectional();

    cout << "\n=== Simulated Output Redirection ===" << endl;
    simulate_redirection();

    return 0;
}