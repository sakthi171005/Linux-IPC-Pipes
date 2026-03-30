#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <string.h> 
#include <fcntl.h> 
#include <unistd.h>
#include <sys/wait.h>

void server(int, int); 
void client(int, int); 

int main() { 
    int p1[2], p2[2], pid; 

    pipe(p1); 
    pipe(p2); 

    pid = fork(); 

    if (pid == 0) { 
        // Child process - Server
        close(p1[1]); 
        close(p2[0]); 
        server(p1[0], p2[1]); 
        close(p1[0]);
        close(p2[1]);
        exit(0);
    } 

    // Parent process - Client
    close(p1[0]); 
    close(p2[1]); 
    client(p1[1], p2[0]); 

    close(p1[1]);
    close(p2[0]);

    wait(NULL); 
    return 0; 
} 

void server(int rfd, int wfd) { 
    int n; 
    char fname[2000]; 
    char buff[2000];

    // Read filename
    n = read(rfd, fname, sizeof(fname));
    if (n <= 0) return;

    fname[n] = '\0';

    // Open file
    int fd = open(fname, O_RDONLY);
    if (fd < 0) { 
        char *msg = "Can't open file\n";
        write(wfd, msg, strlen(msg)); 
    } else { 
        while ((n = read(fd, buff, sizeof(buff))) > 0) {
            write(wfd, buff, n);
        }
        close(fd);
    } 
}

void client(int wfd, int rfd) {
    int n; 
    char fname[2000];
    char buff[2000];

    printf("Enter filename: ");
    scanf("%1999s", fname);

    // Send only actual length
    write(wfd, fname, strlen(fname));

    // Read and print response
    while ((n = read(rfd, buff, sizeof(buff))) > 0) {
        write(1, buff, n);
    }
}
 
