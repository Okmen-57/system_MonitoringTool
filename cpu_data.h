#ifndef CPU_DATA_H
#define CPU_DATA_H

#include <sys/types.h>


typedef struct CPU {
    long long util;  // util time
    long long idle;  // idle time
} CPU;

float getCPUUsage(CPU *cpu_info1, CPU* cpu_info2);

void childCPUCollector(long tdelay, int samples, int cpuPipeWriteFd);

#endif
