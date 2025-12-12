
#ifndef CORES_DATA_H
#define CORES_DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <sys/types.h>


typedef struct CoresInfo{
    int num_cores;
    float max_freq;
} CoresInfo;

void getCoreInfo(CoresInfo* core_info);

#endif