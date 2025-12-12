#include "display.h"

#include <stdio.h>
#include <math.h>

#define Y_AXIS_COL   5
#define AXIS_LABEL_WIDTH 8

void moveCursor(int row, int col) {
    printf("\033[%d;%dH", row, col);
    fflush(stdout);
}

// cleares the screen
void clearScreen() {
    printf("\033[2J\033[H");
    fflush(stdout);
}

/* Memory Graph */

void createMemGraph(int samples, float maxMemGB, int memGraphTop, int memGraphHeight){
    ///_|> descry: sets up the skeleton of the memory usage graph by printing the axes and labels
    ///_|> samples: total number of samples to be plotted, type int
    ///_|> maxMemGB: the total memory in GB, type float
    ///_|> memGraphTop: the top row in the terminal where the memory graph begins, type int
    ///_|> memGraphHeight: the vertical height of the memory grap, type int
    ///_|> returning: this function does not return anything

    // displays the title
    moveCursor(memGraphTop - 2, 2);
    printf("v Memory        %.2f ", maxMemGB);

    moveCursor(memGraphTop - 1, Y_AXIS_COL - 1);
    printf("%.0f", maxMemGB);

    moveCursor(memGraphTop - 1, Y_AXIS_COL + 2);
    printf("GB |");

    // Displays the y-axis
    for (int row = 0; row < memGraphHeight; row++){
        int terminalRow = memGraphTop + row;
        moveCursor(terminalRow, Y_AXIS_COL + AXIS_LABEL_WIDTH - 3);
        printf("|");
    }

    int bottomAxisRow = memGraphTop + memGraphHeight - 1;
    moveCursor(bottomAxisRow, Y_AXIS_COL);
    printf("0 GB  ");
    
    // Displays the x-axis
    for (int i = 0; i < samples; i++){
        printf("-");
    }
    fflush(stdout);
    printf("\n");
}

void updateMemoryGraph(int sampleIndex, float memUsedGB, float maxMemGB,int memGraphTop, int memGraphStartCol, int memGraphHeight){
    ///_|> descry: updates the memory usage graph with a new data point for a given sample index
    ///_|> sampleIndex: the current sample position on the x-axis, type int
    ///_|> memUsedGB: the current used memory in GB, type float
    ///_|> maxMemGB: the total memory capacity used for scaling the graphs y-axis, type float
    ///_|> memGraphTop: the top row in the terminal where the memory graph starts, type int
    ///_|> memGraphStartCol: the column where the memory graph’s bars begin, type int
    ///_|> memGraphHeight: the height of the memory graph, type int
    ///_|> returning: this function does not return anything

    if (maxMemGB < 0.01f) maxMemGB = 0.01f;

    // calculates which row to print
    float levelStep = maxMemGB / memGraphHeight;
    int barHeight = (int)ceil(memUsedGB / levelStep);

    if (barHeight < 1) barHeight = 1;
    if (barHeight > memGraphHeight) barHeight = memGraphHeight;

    // Calculates the column to print
    int targetRow = (memGraphTop + memGraphHeight - barHeight);
    int targetCol = memGraphStartCol + sampleIndex;

    // plots the memory usage
    moveCursor(targetRow, targetCol);
    printf("#");
    fflush(stdout);
}

void createCPUGraph(int samples, int cpuGraphTop, int cpuGraphHeight){
    ///_|> descry: creates the skeleten of teh CPU usage graph, printing axes and labels for percentage
    ///_|> samples: total number of samples to be plotted, type int
    ///_|> cpuGraphTop: the top row in the terminal where the CPU graph begins, type int
    ///_|> cpuGraphHeight: the vertical height of the CPU graph, type int
    ///_|> returning: this function does not return anything

    moveCursor(cpuGraphTop - 2, 2);
    printf("v CPU                  ");

    moveCursor(cpuGraphTop - 1, Y_AXIS_COL + 1);
    printf("100%%|");

    // Creates the y-axis
    for (int row = 0; row < cpuGraphHeight - 1; row++){
        int terminalRow = cpuGraphTop + row;
        moveCursor(terminalRow, Y_AXIS_COL + AXIS_LABEL_WIDTH - 3);
        printf("|");
    }

    int bottomAxisRow = cpuGraphTop + cpuGraphHeight - 1;
    moveCursor(bottomAxisRow, Y_AXIS_COL + 1);
    printf(" 0%%  ");

    // creates the x-axis
    for (int i = 0; i < samples; i++){
        printf("-");
    }
    fflush(stdout);
    printf("\n");
}

void updateCPUGraph(int sampleIndex, float cpuUsagePct, int cpuGraphTop, int cpuGraphStartCol, int cpuGraphHeight){
    ///_|> descry: updates the CPU usage graph with a new data point for a given sample index
    ///_|> sampleIndex: the current sample number / x-axis coordinate, type int
    ///_|> cpuUsagePct: the CPU usage percentage to be plotted, type float
    ///_|> cpuGraphTop: the top row in the terminal for the CPU graph, type int
    ///_|> cpuGraphStartCol: the column where the CPU graphs bars begin, type int
    ///_|> cpuGraphHeight: the height of the CPU graph, type int
    ///_|> returning: this function does not return anything

    if (cpuUsagePct < 0.0f)   cpuUsagePct = 0.0f;
    if (cpuUsagePct > 100.0f) cpuUsagePct = 100.0f;

    // calculates the row
    float ratio = cpuUsagePct / 100.0f;
    int barHeight = (int)(ratio * cpuGraphHeight);

    // calculates the column
    int targetRow = (cpuGraphTop + cpuGraphHeight - 2) - barHeight;
    int targetCol = cpuGraphStartCol + sampleIndex;

    // plots teh cpu percentage
    moveCursor(targetRow, targetCol);
    printf(":");
    fflush(stdout);
}

void create_cores_graph(int num_cores, float max_freq, int graphTop){
    ///_|> descry: draws a diagram representing the number of detected CPU cores and their max frequency
    ///_|> num_cores: the total number of detected cores, type int
    ///_|> max_freq: maximum CPU frequency in GHz for the first core, type float
    ///_|> graphTop: the row in the terminal where the core diagram should start, type int
    ///_|> returning: this function does not return anything

    moveCursor(graphTop, 1);
    printf("v Number of Cores: %d @ %.2f GHz", num_cores, max_freq > 0.0f ? max_freq : 1.0f);

    int cols = 4;
    int rows = (num_cores + cols - 1) / cols;

    // Draws the cores graph, divides the cores to rows of 4
    for (int r = 0; r < rows; r++) {
        moveCursor(graphTop + 2 + (r * 2), 2);

        for (int c = 0; c < cols; c++) {
            if ((r * cols + c) < num_cores){
                printf("+---");
            }
        }
        printf("+\n");

        moveCursor(graphTop + 3 + (r * 2), 2);
        for (int c = 0; c < cols; c++) {
            if ((r * cols + c) < num_cores){
                printf("|   ");
            }
        }
        printf("|\n");
    }

    moveCursor(graphTop + 2 + (rows * 2), 2);
    for (int c = 0; c < cols; c++) {
        if ((rows - 1) * cols + c < num_cores){
            printf("+---");
        }
    }
    printf("+\n");

    fflush(stdout);
}