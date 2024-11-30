#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include "pthread.h"
//---------------Defines-----------------------
#define BUFFER_SIZE (36)
//---------------Prototype-----------------------
pid_t my_fork();
void print_pids(int fd, short unsigned int N, short unsigned int G);
void count_lines(short unsigned int G);
void print_threads(short unsigned int N);
void *printme(void *num);
//--------------------------------------

int main(int argc, char *argv[]) {
    int fd_new;
    short unsigned int N = atoi(argv[1]);
    short unsigned int G = atoi(argv[2]);
    fd_new = open("out.txt", O_CREAT | O_TRUNC | O_RDWR, 0666) ; //Open file dicritor as asked to.
    if(fd_new < 0)
    {
        perror("Error openning file\n");
        exit(EXIT_FAILURE);
    }

    print_pids(fd_new, N, G);    //question 1
    close(fd_new);               //question 2
    count_lines(G);                 //question 3
    print_threads(N);               //question 4

    return 0;
}

//---------------First Task-----------------------
//Create process and check if succeed.
pid_t my_fork(){
    pid_t newStatus = fork();
    if(newStatus == -1){
        perror("Cannot fork\n");
        exit(EXIT_FAILURE);
    }
    return newStatus;
}
//---------------Second Task-----------------------
//In this function every process create N sons till we get to gen on G.
void print_pids(int fd, short unsigned int N, short unsigned int G){
    char str[BUFFER_SIZE + N];
    int g = 0;
    int status;
    pid_t pid;
    for(int n = 0;n < N; )
    {
        if(g > G)
            break;
        pid = my_fork();
        if(pid == 0) {
            n=0;
            g++;
        }
        else
        {
            waitpid(pid, &status, 0);
            if(n == N-1)
            {
                sprintf(str, "My pid is %d. My generation is %d.\n", getpid(), g);
                if (write(fd, str, strlen(str)) == -1)
                    printf("Error while writing to file\n");
            }
            n++;
        }

    }
    if(g > 0)
        kill(getpid(), SIGINT);
}
//---------------Third Task-----------------------
//in this function each process check how many lines are
//exists in the file by its generation(uses grep,wc and system)
void count_lines(short unsigned int G){
    int status;
    int g = G+1;
    pid_t pid;
    char str[45];
    while(g !=0)
    {
        pid = my_fork();
        if(pid == 0) {
            g--;
            printf("Number of lines by processes of generation %d is ", g);
            sprintf(str, "grep -r 'My generation is %d' out.txt | wc -l", g);
            fflush(stdout);
            system(str);
        }
        else{
            waitpid(pid, &status, 0);
            break;
        }
    }
    if(g > 0)
        kill(getpid(), SIGINT);
}
//---------------Fourth Task-----------------------
//The function create N thread and print "hello" - we used join function to make it possible.
void print_threads(short unsigned int N){
    int i;
    int *vals = malloc(N*sizeof(int));
    pthread_t tids[N];
    for (i = 0; i < N; i++)
    {
        vals[i]=i;
        pthread_create(tids+i, NULL, &printme, vals+i);
    }
    for (i = 0; i < N; i++) {
        pthread_join(tids[i], NULL);
        printf("Hi. I'm thread %d\n", i);
    }

    free(vals);
    pthread_exit(NULL);
}
//---------------help func-----------------------

void * printme(void *num)
{
    int *i = (int *)num;
    sleep(*i+1);
}


