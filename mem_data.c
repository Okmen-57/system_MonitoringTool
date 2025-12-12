#include "mem_data.h"

#include <stdio.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <stdlib.h>

void getMemoryUsage(float *used_mem_gb, float *total_mem_gb){
    ///_|> descry: retrieves system memory usage by reading sysinfo() and calculates used and total memory in GB
    ///_|> used_memory_gb: float pointer where the used memory (in GB) will be stored
    ///_|> total_memory_gb: float pointer where the total memory (in GB) will be stored
    ///_|> returning: this function does not return anything

    struct sysinfo info;
    if (sysinfo(&info) != 0){ // default values if unsuccesful
        *used_mem_gb  = 0.0f;
        *total_mem_gb = 1.0f;
        return;
    }

    // total and free bytes of available memory
    double totalBytes = (double)info.totalram * info.mem_unit;
    double freeBytes  = (double)info.freeram  * info.mem_unit;

    // converts to gb
    *total_mem_gb = (float)(totalBytes / (1024.0 * 1024.0 * 1024.0));
    float free_gb = (float)(freeBytes  / (1024.0 * 1024.0 * 1024.0));
    *used_mem_gb = *total_mem_gb - free_gb;
}


void childMemCollector(long tdelay, int samples, int memPipeWriteFd){
    ///_|> descry: collects memory usage in a loop, then writes results to the parent process through a pipe
    ///_|> tdelay: the delay between each sample in microseconds, type long
    ///_|> samples: how many times memory usage is sampled, type int
    ///_|> memPipeWriteFd: file descriptor of pipe for writing memory data to the parent process, type int
    ///_|> returning: this function does not return anything

    for (int i = 0; i < samples; i++){
        // calculate the memory usage
        mem_data memInfo;
        getMemoryUsage(&memInfo.used_gb, &memInfo.total_gb);

        // write to memory pipe
        write(memPipeWriteFd, &memInfo, sizeof(mem_data));
        usleep(tdelay);
    }

    // Close pipe and exit from process
    close(memPipeWriteFd);
    exit(0);
}
