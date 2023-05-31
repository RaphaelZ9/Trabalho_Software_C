#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
void printPai(pid_t pid) {
    printf("Pai: %d\n", pid);
}
void printFilho(pid_t pid) {
    printf("Filho: %d\n", pid);
}
int main(void) {
    int i;
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(0);
    }
    if (pid == 0) {
        printFilho(getpid());
    } else {
        printPai(getpid());
    }
    printf("Este print será executado por ambos os processos \n\n");
    exit(0);
}