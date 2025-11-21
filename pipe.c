 #include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2];
    pid_t pid;
    char write_msg[] = "Hello from Parent!";
    char read_msg[100];

    // Create pipe
    if (pipe(fd) == -1) {
        perror("Pipe failed");
        return 1;
    }

    // Create child process
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid > 0) {
        // Parent process
        close(fd[0]); // Close read end

        // Write message
        write(fd[1], write_msg, strlen(write_msg) + 1);
        close(fd[1]); // Close write end after writing

        printf("Parent: Message sent to child: %s\n", write_msg);
    } else {
        // Child process
        close(fd[1]); // Close write end

        // Read message
        read(fd[0], read_msg, sizeof(read_msg));
        close(fd[0]); // Close read end after reading

        printf("Child: Message received from parent: %s\n", read_msg);

        _exit(0);
    }

    return 0;
}