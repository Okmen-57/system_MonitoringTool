#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>


void setChildPids(pid_t cpu, pid_t mem, pid_t cores);

void SignalHandlers();

void ignoreAction();

#endif