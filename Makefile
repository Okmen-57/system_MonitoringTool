CC = gcc
CFLAGS = -Wall -Werror -std=c99 -D_GNU_SOURCE
LDFLAGS = -lm

TARGET = myMonitoringTool

OBJS = myMonitoringTool.o cpu_data.o mem_data.o display.o parser.o cores_data.o signal_Handler.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
