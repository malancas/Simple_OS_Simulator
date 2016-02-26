CC=g++
CFLAGS= -std=c++11 -O3 -Wall -Werror -Wextra -pedantic

all: jobs

jobs:
	$(CC) $(CFLAGS) main.cpp Process.cpp ReadyQueue.cpp DeviceQueues.cpp Sysgen.cpp Memory.cpp SystemCall.cpp -o jobs
clean:
	rm jobs