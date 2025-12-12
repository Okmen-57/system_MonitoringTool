// signal_Handler.c

#include "signal_Handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

pid_t cpuChildPid = -1;
pid_t memChildPid = -1;
pid_t coresChildPid = -1;

void setChildPids(pid_t cpu, pid_t mem, pid_t cores){
    ///_|> descry: stores the process IDs of the CPU, memory, and cores child processes
    ///_|> cpu: PID of the CPU child, type pid_t
    ///_|> mem: PID of the memory child, type pid_t
    ///_|> cores: PID of the cores child, type pid_t
    ///_|> returning: this function does not return anything

    cpuChildPid = cpu;
    memChildPid = mem;
    coresChildPid = cores;
}

// Moving cursor to row 30 col 10
void move_to_prompt(){
    printf("\033[30;10H");
    fflush(stdout);
}

void handleSIGINT(int signo){
    ///_|> descry: handles the SIGINT signal (Ctrl-C)
    ///_|> signo: the signal number SIGINT, type int
    ///_|> returning: this function does not return anything

    char response[16];

    move_to_prompt();
    printf("Ctrl-C detected. Quit? (y/n): ");
    fflush(stdout);

    //stop children so they don't keep running
    if (cpuChildPid > 0) kill(cpuChildPid, SIGSTOP);
    if (memChildPid > 0) kill(memChildPid, SIGSTOP);
    if (coresChildPid > 0) kill(coresChildPid, SIGSTOP);

    if (fgets(response, sizeof(response), stdin) != NULL){
        if (response[0] == 'y' || response[0] == 'Y'){
            // kill all processes in group (this kills parent too)
            kill(0, SIGTERM);
        } else {
            // resume children
            if (cpuChildPid > 0) kill(cpuChildPid, SIGCONT);
            if (memChildPid > 0) kill(memChildPid, SIGCONT);
            if (coresChildPid > 0) kill(coresChildPid, SIGCONT);
        }
    }
    // clearing promptt line
    printf("\033[A\033[2K\r");
    fflush(stdout); 
}

void SignalHandlers(){
    ///_|> descry: signal handlers for Ctrl-C (SIGINT) and Ctrl-Z (SIGTSTP)
    ///_|> returning: this function does not return anything

    struct sigaction sa_interrupt;
    struct sigaction sa_ignore;

    // SIGINT handler (Ctrl-C)
    memset(&sa_interrupt, 0, sizeof(sa_interrupt));
    sa_interrupt.sa_handler = handleSIGINT;
    sigaction(SIGINT, &sa_interrupt, NULL);

    // SIGTSTP to be ignored (Ctrl-Z)
    memset(&sa_ignore, 0, sizeof(sa_ignore));
    sa_ignore.sa_handler = SIG_IGN;  // Ignore signal
    sigaction(SIGTSTP, &sa_ignore, NULL);
}

void ignoreAction(){
    ///_|> descry: sets the current process SIGINT handler to ignore the signal
    ///_|> returning: this function does not return anything

    struct sigaction ignore_action;
    memset(&ignore_action, 0, sizeof(ignore_action));
    ignore_action.sa_handler = SIG_IGN;
    sigaction(SIGINT, &ignore_action, NULL);
}
