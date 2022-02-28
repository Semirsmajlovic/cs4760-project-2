// Define our includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <time.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/ipc.h>
#include <errno.h>

// Define our void handlers
void timeout_handle();
void termination_handle();
void termination_error();
void timeout_error();
void fork_error();

// Define our global variables
bool * calling_pointer;
int * number_pointer;
int bool_id;
int integer_id;
int process_count;
int * process_id_list = NULL;

int main(int argc, char *argv[])
{
    // Define our variables
    int process_id;
    int option;
    int ss = 100;
    int n_of_p = 20;

    // Detect our signal interruption
    signal(
        SIGINT, 
        termination_handle
    );

    // Define our time identifications
    time_t Time;
    struct tm TimeStructure;

    // Set our time identifiers with appropriate values
    Time = time(NULL);
    TimeStructure = *localtime(&Time);

    // Loop through our options.
    while ((option = getopt(argc, argv, "t:")) != -1) {
        switch (option) {
        case 't':
            ss = atoi(optarg);
            break;
        case '?':
            printf("Invalid argument has been provided, the process is now terminating...\n\n");
            break;
        default:
            break;
        }
    }

    // If we have don't have 4 arguments, print out error.
    n_of_p = atoi(argv[3]);
    if (n_of_p > 20) {
        printf("The number of processes defined exceeds 20, defaulting back to 20.\n");
        n_of_p = 20;
    }

    // Set our process count and allocate our memory
    process_count = n_of_p;
    process_id_list = malloc(
        sizeof(int) * n_of_p
    );
    bool * choosing = malloc(
        sizeof(bool) * n_of_p
    );
    int * number = malloc(
        sizeof(int) * n_of_p
    );
    
    // Initialize our arrays
    key_t bool_key = ftok("Makefile", 'a');
    bool_id = shmget(
        bool_key, 
        sizeof(bool) * n_of_p, 
        IPC_CREAT | 0666
    );

    key_t integer_key = ftok(".", 'a');
    integer_id = shmget(
        integer_key, 
        sizeof(int) * n_of_p, 
        IPC_CREAT | 0666
    );

    // Attach on the shared memory segment
    calling_pointer = (bool *) shmat(
        bool_id, 
        0, 
        0
    );
    number_pointer = (int *) shmat(
        integer_id, 
        0, 
        0
    );

    // Loop through our pointers
    for (int i = 0; i < n_of_p; i++) {
        calling_pointer[i] = false;
    }
    for (int i = 0; i < n_of_p; i++) {
        number_pointer[i] = 0;
    }
    
    // Add spacing for our execution
    printf("\n");

    // Loop through our number of processes
    for (int i = 0; i < n_of_p; i++) {
        
        // Allocate our time identifiers
        Time = time(NULL);
        TimeStructure = *localtime(&Time);

        // Output execution of child process
        printf("Child process %d is being forked..\n", i + 1);

        // Set our process execution
        process_id = fork();
        if (process_id == 0) {
            char * numprocs = malloc(sizeof(char) * 3);
            sprintf(
                numprocs, 
                "%d", 
                n_of_p
            );
            char * pnum = malloc(sizeof(char) * 3);
            sprintf(
                pnum, 
                "%d", 
                i + 1
            );
            execl(
                "./slave", 
                "./slave", 
                pnum, 
                numprocs, 
                NULL
            );
        } else {
            process_id_list[i] = process_id;
        }
    }

    // Wait for all processes to terminate
    while (wait(NULL) > 0);

    // Detach from the calling process address
    shmdt(
        calling_pointer
    );
    shmdt(
        number_pointer
    );

    // Detach our shared memory.
    shmctl(
        bool_id, 
        IPC_RMID, 
        NULL
    );
    shmctl(
        integer_id, 
        IPC_RMID, 
        NULL
    );

    // Set our time identifiers back to normal
    Time = time(NULL);
    TimeStructure = *localtime(&Time);

    exit(0);
}

void timeout_handle()
{
    for (int i = 0; i < process_count; i++) {
        kill(
            process_id_list[i], 
            SIGTERM
        );
    }
    while(wait(NULL) > 0);
    shmdt(calling_pointer);
    shmdt(number_pointer);
    shmctl(
        bool_id, 
        IPC_RMID, 
        NULL
    );
    shmctl(
        integer_id, 
        IPC_RMID, 
        NULL
    );
    printf("The process has surpassed the timeout, the execution will now terminate...\n\n");
    exit(-1);
}

void timeout_error()
{ 
    for (int i = 0; i < process_count; i++) {
        kill(
            process_id_list[i], 
            SIGTERM
        );
    }
    while(wait(NULL) > 0);
    shmdt(calling_pointer);
    shmdt(number_pointer);
    shmctl(
        bool_id, 
        IPC_RMID, 
        NULL
    );
    shmctl(
        integer_id, 
        IPC_RMID, 
        NULL
    );
    printf("The process has surpassed the timeout, the execution will now terminate...\n\n");
    exit(-1);
}

void termination_handle()
{
    termination_error();
}

void termination_error()
{ 
    for (int i = 0; i < process_count; i++)
    {
        kill(
            process_id_list[i], 
            SIGTERM
        );
    }
    while(wait(NULL) > 0);
    shmdt(
        calling_pointer
    );
    shmdt(
        number_pointer
    );
    shmctl(
        bool_id, 
        IPC_RMID, 
        NULL
    );
    shmctl(
        integer_id, 
        IPC_RMID, 
        NULL
    );
    printf("Termination has been invoked using CTRL + C, the process will now terminate...\n");
    exit(-1);
}

void fork_error()
{
    shmdt(calling_pointer);
    shmdt(number_pointer);
    shmctl(
        bool_id, 
        IPC_RMID, 
        NULL
    );
    shmctl(
        integer_id, 
        IPC_RMID, 
        NULL
    );
    for (int i = 0; i < process_count; i++) {
        kill(
            process_id_list[i], 
            SIGTERM
        );
    }
    printf("There was an error when forking, the process will now terminate...\n");
    exit(-1);
}