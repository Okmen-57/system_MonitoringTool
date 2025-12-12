#ifndef MEM_DATA_H
#define MEM_DATA_H

/**
* A struct to store memory usage data (in GB)
*/
typedef struct {
    float used_gb;
    float total_gb;
} mem_data;

void getMemoryUsage(float *used_memory_gb, float *total_memory_gb);

void childMemCollector(long tdelay, int samples, int memPipeWriteFd);

#endif
