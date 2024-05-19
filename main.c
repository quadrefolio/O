
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MEMORY_SIZE 61

typedef struct 
{
    int min;
    int max;
    
}MemoryBoundaries;


typedef struct 
{
    int pID;
    char state [32];
    int PC;
    MemoryBoundaries Memoryboundaries;
}PCB;

typedef struct 
{
    char name [32];
    char data [64];
}Memory;


int quanta;
int arrival_1;
int arrival_2;
int arrival_3;
int clock;
Memory *memoryArray;

void initializeMemoryArray() {
    memoryArray = (Memory *)malloc(MEMORY_SIZE * sizeof(Memory));
    if (memoryArray == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    // Initialize the structs in the array
    for (int i = 0; i < MEMORY_SIZE; i++) {
        snprintf(memoryArray[i].name, 64 , "");
        snprintf(memoryArray[i].data, 64, "");
    }

}

void addProcess(int processID, int arrivalTime, const char *filename) {
    PCB newProcess;
    newProcess.pID = processID;
    newProcess.PC = 0;
    strcpy(newProcess.state, "Ready");

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }


    char line[64];
    int lineCount = 0;
    while (fgets(line, sizeof(line), file)) {
        snprintf(memoryArray[lineCount].name, 64 , "Instruction");
        snprintf(memoryArray[lineCount].data, 64, line);
        printf("%s: ",memoryArray[lineCount].name);
        printf("%s \n",memoryArray[lineCount].data);
        lineCount++;

    }

    fclose(file);

}









int main(){
    initializeMemoryArray();
    addProcess(1, 0,"Program_1.txt");
    return 0;
}