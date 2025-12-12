#include "cores_data.h"

void getCoreInfo(CoresInfo* core_info){
    ///_|> descry: reads the number of cores and maximum frequency from relavant files,
    /// and stores them in the cores struct
    ///_|> core_info: pointer to a CoresInfo struct where the number of cores 
    /// and max frequency are stored, type CoresInfo*
    ///_|> returning: this function does not return anything

    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (!fp){
        core_info->num_cores = 0;
        core_info->max_freq  = 0.0f;
        return;
    }

    char line[256];
    int cores = 0;

    // Counts the number cores by iterating through lines that contain 'processor'
    while (fgets(line, sizeof(line), fp)){ 
        if (strstr(line, "processor") != NULL){
            cores++;
        }
    }
    fclose(fp);
    
    // Get max frequency of cpu
    FILE *freqFile = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", "r");
    if (freqFile){
        int freqKHz;
        if (fscanf(freqFile, "%d", &freqKHz) == 1){
            core_info->max_freq = freqKHz / 1000000.0f;  // converting kHz to GHz
        }
        fclose(freqFile);
    }else{
        core_info->max_freq = 0.0f;
    }

    core_info->num_cores = cores;
}