// Define includes
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

int main(int argc, char *argv[])
{
    // File definitions
    FILE *logfile;
    FILE *cstest;

    // Define our keys
    key_t bool_key = ftok(
        ".", 
        'a'
    );
    key_t integer_key = ftok(
        ".", 
        'a'
    );

    // Define our variables
    bool *bool_pointer;
    int *integer_pointer;
    int bool_id;
    int integer_id;

    // Process handlers
    int process_count = atoi(argv[2]);
    int process_identifier = atoi(argv[1]);

    // Return our identifiers
    bool_id = shmget(
        bool_key, 
        sizeof(bool) * process_count, 
        IPC_EXCL
    );
    integer_id = shmget(
        integer_key, 
        sizeof(int) * process_count, 
        IPC_EXCL
    );

    // Attach our shared memory
    bool_pointer = (bool *) shmat(
        bool_id, 
        0, 
        0
    );
    integer_pointer = (int *) shmat(
        integer_id, 
        0, 
        0
    );

    char *logname = malloc(
        sizeof(char) * 11
    );
    sprintf(
        logname, 
        "logfile.%d", 
        process_identifier
    );

    // Initialize time variables to output Systemtime in logfile
    time_t Time;
    struct tm TimeStructure;

    for (int i = 0; i < 5; i++) {
        bool_pointer[process_identifier - 1] = 1;
        int maximum = 0;
        for (int k = 0; k < process_count; k++)
        {
            if (integer_pointer[k] > maximum)
            {
                maximum = integer_pointer[k];
            }
        }

        // Set our pointers
        integer_pointer[
            process_identifier - 1
        ] = 1 + maximum;
        bool_pointer[
            process_identifier - 1
        ] = 0;
        
        // Loop through our process count
        for (int j = 0; j < process_count; j++) {
            while (bool_pointer[j]);
            while ((integer_pointer[j] != 0) && (integer_pointer[j] < integer_pointer[process_identifier - 1] || 
            (integer_pointer[j] == integer_pointer[process_identifier - 1] && j < process_identifier - 1)));
        }

        // Execute opening logfile and cstest
        logfile = fopen(
            logname, 
            "a"
        );
        cstest = fopen(
            "cstest", 
            "a"
        );

        // Define our time identifiers
        Time = time(NULL);
        TimeStructure = *localtime(&Time);

        // Print to logfile
        fprintf(
            logfile, 
            "[%02d:%02d:%02d]: Process %d is executing...\n", 
            process_identifier, 
            TimeStructure.tm_hour, 
            TimeStructure.tm_min, 
            TimeStructure.tm_sec
        );
        
        // Initialize time variables to output Systemtime in logfile
        Time = time(NULL);
        TimeStructure = *localtime(&Time);

        // Print to logfile
        fprintf(
            cstest, 
            "[%02d:%02d:%02d]: Process %d has modified the file %d\n", 
            TimeStructure.tm_hour, 
            TimeStructure.tm_min, 
            TimeStructure.tm_sec, 
            integer_pointer[process_identifier - 1], 
            process_identifier
        );

        // Print to logfile
        fprintf(
            logfile, 
            "[%02d:%02d:%02d]: Process %d is being written to cstest.\n", 
            process_identifier, 
            TimeStructure.tm_hour, 
            TimeStructure.tm_min, 
            TimeStructure.tm_sec
        );

        // Reset our time identifiers
        Time = time(NULL);
        TimeStructure = *localtime(&Time);

        // Print to logfile
        fprintf(
            logfile, 
            "Execution has ended.\n"
        );

        // Close out logfile and csfile
        fclose(logfile);
        fclose(cstest);
    }

    exit(0);
}