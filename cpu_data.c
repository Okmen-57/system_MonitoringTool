#include "cpu_data.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void getCPUInfo(CPU* cpu_info){
    FILE *fp = fopen("/proc/stat", "r");

    if (fp == NULL){
        fprintf(stderr, "Error opening /proc/stat");
        return;
    }
    // gather the cpu usage from /proc/stat
    long long user, nice, system, idle, iowait, irq, softirq;
    if (fscanf(fp, "cpu %lld %lld %lld %lld %lld %lld %lld",
        &user, &nice, &system, &idle, &iowait, &irq, &softirq) != 7){ //error checking
        fclose(fp);
        fprintf(stderr, "Error scanning /proc/stat\n");
        cpu_info->util = 0;
        cpu_info->idle = 0;
        return;
    }
    fclose(fp);

    // Calculating utilization and idle times for CPU
    long long util = user + nice + system + iowait + irq + softirq;
    
    cpu_info->util = util;
    cpu_info->idle = idle;
}

float getCPUUsage(CPU *cpu_info1, CPU* cpu_info2){
    ///_|> descry: calculates the CPU usage percentage based on two CPU usages
    ///_|> cpu_info1: pointer to the first CPU usage struct containing utilization and idle times, type CPU*
    ///_|> cpu_info2: pointer to the second CPU usage struct containing utilization and idle times, type CPU*
    ///_|> returning: returns the calculated CPU usage as a percentage

    if (cpu_info1 == NULL|| cpu_info2 == NULL){
        // If pointers are invalid we store 0 and return
        return 0;
    }

    // Calculate new totals
    long long T1 = cpu_info1->util + cpu_info1->idle;
    long long T2 = cpu_info2->util + cpu_info2->idle;
    long long diff_total = T2 - T1;
    long long diff_util = cpu_info2->util - cpu_info1->util;

    float cpu_usage = ((float)diff_util / (float)diff_total) * 100.0f;

    return cpu_usage;
}


void childCPUCollector(long tdelay, int samples, int cpuPipeWriteFd){
    ///_|> descry: collects CPU usage percentages in a loop, then writes results to the parent process through a pipe
    ///_|> tdelay: the delay between each CPU sample in microseconds, type long
    ///_|> samples: how many times CPU usage is sampled, type int
    ///_|> cpuPipeWriteFd: file descriptor for writing CPU usage data to the parent, type int
    ///_|> returning: this function does not return anything but exits from process


    for (int i = 0; i < samples; i++){
        // declare new CPU structs to store cpu usages
        CPU cpu_info1, cpu_info2;

        // gather the two cpu usages with tdelay in between
        getCPUInfo(&cpu_info1);
        usleep(tdelay);
        getCPUInfo(&cpu_info2);

        // calculate the CPU usage and write to pipe
        float cpu_usage;
        cpu_usage = getCPUUsage(&cpu_info1, &cpu_info2);
        write(cpuPipeWriteFd, &cpu_usage, sizeof(float));
    }

    // close pipe and exit the process
    close(cpuPipeWriteFd);
    exit(0);
}
