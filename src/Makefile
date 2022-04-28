CC = nvcc 
SRCS = ./*.cpp
INC = ./
OPTS = -std=c++17 -O3 

EXEC = ./kmeans

all: compile

compile:
	$(CC) $(SRCS) --compiler-options $(OPTS) -I$(INC) -o $(EXEC)

