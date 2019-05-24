#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFSIZE 50

typedef struct dStruct {
    char        *state;
    long int    sysTime;
    long int    userTime;
} DataStruct;

/* This function reads the process state, its used system time and used user time
    from the specified statFile and fills the specified data structure. If the file
    cannot be read (i.e. the monitored process no longer exists, reurns NULL pointer,
    otherwise returns pointer to the procided data structure. */
DataStruct *getData(char *statFile, DataStruct *data) {
    FILE *fp;
    char state;

    fp = fopen(statFile, "r");
    if (!fp) {
        printf("procmon: Cannot open %s, the monitored process is not running any more.\n", statFile);
        return NULL;
    }
    fscanf(fp, "%*s %*s %c %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %ld %ld",
        &state, &(data->userTime), &(data->sysTime));

    switch(state) {
        case 'R': data->state = (char*)"Running          "; break;
        case 'S': data->state = (char*)"Sleeping(memory) "; break;
        case 'D': data->state = (char*)"Sleeping(disk)   "; break;
        case 'Z': data->state = (char*)"Zombie           "; break;
        case 'T': data->state = (char*)"Traced/Stopped   "; break;
        case 'W': data->state = (char*)"Paging           "; break;
    }
    fclose(fp);
    return data;
}

/*--------------------------------------------------------------------------
Function: main
Description: The main function processes command line arguments and invokes
             appropriate functions to read in data from kernel and write
             to the standard output the information collected. This should
             be a relatively simple function.
--------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
    DataStruct  data;
    char        statFile[BUFFSIZE];
    int         t;

    if (argc != 2) {
        printf("Usage: procmon PID\n where PID is a process ID of a running process.\n");
        exit(-1);
    }
    sprintf(statFile, "/proc/%s/stat", argv[1]);

    printf("\n\n        Monitoring %s:\n\n", statFile);
    printf("Time        State           SysTm    UsrTm\n");
    for(t=0; ;t++) {
        if (getData(statFile, &data) == NULL)
            exit(0);
        else
            printf("%3d     %s %6ld %8ld\n", t, data.state, data.sysTime, data.userTime);
        sleep(1);
    }
}
