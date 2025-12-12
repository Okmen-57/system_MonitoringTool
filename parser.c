#include "parser.h"

// Check to see if a given string is numeric
int isNumeric(const char *str) {
    if (*str == '\0')  // Check for empty string
        return 0;

    while (*str) {
        if (!isdigit((char)*str)) // Check if character is not a digit
            return 0; // Return 0 if str is not numeric
        str++;
    }
    return 1; // Return 1 if all characters are digits
}

void parseArgs(int argc, char *argv[], int *samples, long *tdelay, int *flag_mem, int *flag_cpu, int *flag_cores){
    ///_|> descry: parses through the command-line arguments for the program, 
    // sets default values for samples and tdelay, and assigns relevant flags
    ///_|> argc: the number of command-line arguments, of type int
    ///_|> argv: the array of command-line arguments, of type char**
    ///_|> samples: int pointer that will be assigned the number of samples
    ///_|> tdelay: long pointer that will be assigned the sampling delay in microseconds
    ///_|> flag_mem: int pointer that indicates whether to display memory usage
    ///_|> flag_cpu: int pointer that indicates whether to display CPU usage, type int*
    ///_|> flag_cores: int pointer that indicates whether to display core info
    ///_|> returning: this function does not return anything

    // default values for samples and tdelay
    *samples = DEFAULT_SAMPLES;
    *tdelay = DEFAULT_TDELAY;

    // default values for metric flags
    *flag_mem = 0;
    *flag_cpu = 0;
    *flag_cores = 0;

    int count = 0;

    for (int i = 1; i < argc; i++){
        // positional argument checker, assuming postional arguments come first
        if (i == 1 && isNumeric(argv[i])){
            *samples = atoi(argv[1]);
            count++;
        }else if (i == 2 && isNumeric(argv[i]) && count == 1){
            *tdelay = atol(argv[2]);
        }

        else if (strncmp(argv[i], "--samples=", 10) == 0) {
            *samples = atoi(argv[i] + 10);
        }
        else if (strncmp(argv[i], "--tdelay=", 9) == 0) {
            *tdelay = atol(argv[i] + 9);
        }
        else if (strcmp(argv[i], "--memory") == 0) {
            *flag_mem = 1;
        }
        else if (strcmp(argv[i], "--cpu") == 0) {
            *flag_cpu = 1;
        }
        else if (strcmp(argv[i], "--cores") == 0) {
            *flag_cores = 1;
        } else{
            fprintf(stderr, "Unknown flag: %s\n", argv[i]); // non valid flag provided
            exit(1);
        }
    }
}

// helper function to safely read
ssize_t safeRead(int fd, void *buf, size_t count){
    int n;
    do{
        n = read(fd, buf, count);
    } while (n < 0 && errno == EINTR);
    return n;
}

