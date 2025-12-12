#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <errno.h>

#include "parser.h"
#include "display.h"
#include "cpu_data.h"
#include "mem_data.h"
#include "cores_data.h"
#include "signal_Handler.h"

int main(int argc, char *argv[]){
    int samples;
    long tdelay;
    int flag_mem, flag_cpu, flag_cores;
    parseArgs(argc, argv, &samples, &tdelay, &flag_mem, &flag_cpu, &flag_cores);

    // If no specified flags we default to all flags
    bool no_flags = (!flag_mem && !flag_cpu && !flag_cores);
    if (no_flags){
        flag_mem = 1;
        flag_cpu = 1;
        flag_cores = 1;
    }

    //calling signal handler
    SignalHandlers();

    // separate pipes for cpu, memory and core processes
    int cpuPipe[2];
    int memPipe[2];
    int coresPipe[2];

    // Error checking for pipe creation
    if (flag_cpu && pipe(cpuPipe) < 0) {
        perror("Error creating cpu pipe");
        return 1;
    }
    if (flag_mem && pipe(memPipe) < 0) {
        perror("error creating mem pipe");
        return 1;
    }
    if (flag_cores && pipe(coresPipe) < 0) {
        perror("Erro creating core pipe");
        return 1;
    }

    // Fork CPU child if flag provided
    pid_t cpuChildPid = -1;
    if (flag_cpu){
        cpuChildPid = fork();
        if (cpuChildPid < 0){
            perror("Error forking cpu child");
            return 1;
        }
        if (cpuChildPid == 0){ //CPU child
            // Ignore signal in child
            ignoreAction();

            close(cpuPipe[0]); // child does not read from the pipe

            // calling childCPUCollector and passing pipes write FD
            childCPUCollector(tdelay, samples, cpuPipe[1]);
        }
    }

    // Fork Memory child if flag provided
    pid_t memChildPid = -1;
    if (flag_mem){
        memChildPid = fork();
        if (memChildPid < 0){
            perror("Error forking memory child");
            return 1;
        }
        if (memChildPid == 0){ // memory child
            // Ignore signal in child
            ignoreAction();

            // child does not read from the pipe
            close(memPipe[0]);

            // calling childmemCollector and passing pipes write FD
            childMemCollector(tdelay, samples, memPipe[1]);
        }
    }

    // Fork cores child if flag provided
    pid_t coresChildPid = -1;
    if (flag_cores){
        coresChildPid = fork();
        if (coresChildPid < 0){
            perror("Error forkin cores child");
            return 1;
        }
        if (coresChildPid == 0){
            // Child does not read from cores Pipe so close read end
            close(coresPipe[0]);

            // ignore signal in child
            ignoreAction();

            // get core info in core process
            CoresInfo core_info;
            getCoreInfo(&core_info);
            write(coresPipe[1], &core_info, sizeof(core_info)); //Writing core info to parent proc

            // No longer needed so we exit
            close(coresPipe[1]);
            exit(0);
        }
    }

    // sending the child pids to signal handler
    setChildPids(cpuChildPid, memChildPid, coresChildPid);

    // Parent code

    // Parent does not write to the CPU, mem, and core pipes so we close the write end
    if (flag_cpu) close(cpuPipe[1]);
    if (flag_mem) close(memPipe[1]);
    if (flag_cores) close(coresPipe[1]);

    // Preparindg for displaying
    clearScreen();
    moveCursor(1, 1);
    printf("Nbr of samples: %d -- every %ld microSecs (%.3f secs)\n", samples, tdelay, (float)tdelay / 1e6);
           
    int currentGraphRow = 5;
    int memGraphTop = 0, cpuGraphTop = 0;
    int memGraphHeight = 12;
    int cpuGraphHeight = 10;

    int memGraphStartCol = 3 + 8;
    int cpuGraphStartCol = 3 + 8;

    float memTotal = 0.0f;

    if (flag_mem){
        float memUsed=0.0f;
        getMemoryUsage(&memUsed, &memTotal);

        float maxMem = (float)((int)(memTotal + 0.5f));
        if (maxMem < 1.0f) maxMem = 1.0f;

        memGraphTop = currentGraphRow;
        createMemGraph(samples, maxMem, memGraphTop, memGraphHeight);
        currentGraphRow += memGraphHeight + 2;
    }

    if (flag_cpu) {
        cpuGraphTop = currentGraphRow;
        createCPUGraph(samples, cpuGraphTop, cpuGraphHeight);
        currentGraphRow += cpuGraphHeight + 2;
    }

    // reading data from children and update graphs
    for (int i = 0; i < samples; i++){ // main loop for displaying mem and CPU graphs
        // CPU reading info from pipe
        if (flag_cpu) {
            float usage;
            ssize_t n = safeRead(cpuPipe[0], &usage, sizeof(usage));
            if (n == sizeof(usage)){
                // update CPU usage
                moveCursor(cpuGraphTop - 2, 14);
                printf("%6.2f %%", usage);

                updateCPUGraph(i, usage, cpuGraphTop, cpuGraphStartCol, cpuGraphHeight);
            }
        }

        // MEM reading from pipe
        if (flag_mem) {
            mem_data memInfo;
            ssize_t n = safeRead(memPipe[0], &memInfo, sizeof(mem_data));
            if (n == sizeof(memInfo)){
                // Updating the memory usage in the graph
                moveCursor(memGraphTop - 2, 15);
                printf("%6.2f GB", memInfo.used_gb);

                updateMemoryGraph(i, memInfo.used_gb, memInfo.total_gb, memGraphTop, memGraphStartCol, memGraphHeight);
            }
        }
    }

    // If core flag is provided, we display its diagram
    if(flag_cores){
        CoresInfo cores_info;
        ssize_t n = safeRead(coresPipe[0], &cores_info, sizeof(cores_info));
        
        if(n == sizeof(cores_info)){
            currentGraphRow += 2;
            create_cores_graph(cores_info.num_cores, cores_info.max_freq, currentGraphRow);
        }
    }

    // Waiting for children to finish in the main process
    if (flag_cpu){
        waitpid(cpuChildPid, NULL, 0);
    }
    if (flag_mem){
        waitpid(memChildPid, NULL, 0);
    }
    if(flag_cores){
        waitpid(coresChildPid, NULL, 0);
    }

    moveCursor(currentGraphRow + 14, 1);
    printf("Done\n");
    return 0;
}
