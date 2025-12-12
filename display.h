#ifndef DISPLAY_H
#define DISPLAY_H


void moveCursor(int row, int col);
void clearScreen();


void createMemGraph(int samples, float maxMemGB, int memGraphTop, int memGraphHeight);

void updateMemoryGraph(int sampleIndex, float memUsedGB, float maxMemGB, int memGraphTop, int memGraphStartCol, int memGraphHeight);

void createCPUGraph(int samples, int cpuGraphTop, int cpuGraphHeight);

void updateCPUGraph(int sampleIndex, float cpuUsagePct, int cpuGraphTop, int cpuGraphStartCol, int cpuGraphHeight);

void create_cores_graph(int num_cores, float max_freq, int graphTop);

#endif
