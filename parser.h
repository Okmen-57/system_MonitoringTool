#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define DEFAULT_SAMPLES       20
#define DEFAULT_TDELAY        500000

void parseArgs(int argc, char *argv[], int *samples, long *tdelay, int *flag_mem, int *flag_cpu, int *flag_cores);

ssize_t safeRead(int fd, void *buf, size_t count);

#endif
