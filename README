# System Monitoring Tool

## Overview
This project implements a real-time Linux system monitoring tool that displays memory usage, CPU usage, and core information. The user can select which metrics to display, how many samples to collect, and the delay between samples. The tool uses child processes to gather data in parallel, communicates via pipes, and installs custom handlers for `Ctrl-C` and `Ctrl-Z`.

---

## Features
- **Real-time monitoring**
  - Memory usage  
  - CPU usage  
  - Core count and maximum frequency  
- **Configurable sampling**
  - `--samples=N`
  - `--tdelay=N`
- **Parallel metric collection** using separate child processes  
- **Signal handling**
  - `Ctrl-Z` is ignored  
  - `Ctrl-C` pauses execution and prompts whether to quit or continue  
- **Text-based graphs** for memory and CPU usage

---

## Design Overview

### Architecture
- **Parent process**
  - Parses arguments
  - Creates pipes
  - Forks child processes
  - Receives data from children
  - Updates display graphs

- **Child processes (one per metric)**
  - Collect metric data (memory, CPU, cores)
  - Write results to parent via pipe
  - Exit after sample completion

### Signal Handling
- **SIGINT (`Ctrl-C`)**  
  Pauses the program and asks the user whether to quit or resume.

- **SIGTSTP (`Ctrl-Z`)**  
  Ignored entirely.

---

## Compilation

The compilation can be done using a Makefile.

To compile use:

	make

To remove the object files and the executable use:

	make clean

