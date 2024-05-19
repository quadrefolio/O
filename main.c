
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MEMORY_SIZE 61
#define NUM_INST 3


//To define the boundaries of a process
typedef struct 
{
    int min;
    int max;
} MemoryBoundaries;

//To define the Process Control Block (and thus, PCB)
typedef struct 
{
    int pID;
    char state [32];
    int PC;
    MemoryBoundaries Memoryboundaries;
} PCB;

//Memory structure to store the data of the process
typedef struct 
{
    char name [32];
    char data [64];
} Memory;

//Global variables
int quanta;
int arrival_1;
int arrival_2;
int arrival_3;
int clock;
int inst =1;
Memory *memoryArray;
PCB programs[NUM_INST];

int memIndex = 0;

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

void printMemoryArray() {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        printf("Memory %d\n", i);
        printf("Name: %s\n", memoryArray[i].name);
        printf("Data: %s\n", memoryArray[i].data);
        printf("\n");
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

    if (inst == 1){
        newProcess.Memoryboundaries.min = 0;
    }    
    else {
        newProcess.Memoryboundaries.min = programs[processID-1].Memoryboundaries.max + 1;
    }

    snprintf(memoryArray[memIndex].name, 64 , "Process ID");
    snprintf(memoryArray[memIndex].data, 64, "%d", processID);

    memIndex++;

    snprintf(memoryArray[memIndex].name, 64 , "Process state");
    snprintf(memoryArray[memIndex].data, 64, "READY");

    memIndex++;

    snprintf(memoryArray[memIndex].name, 64 , "PC");
    snprintf(memoryArray[memIndex].data, 64, "%d", newProcess.PC);
    int PCindex = memIndex;

    memIndex++;

    snprintf(memoryArray[memIndex].name, 64 , "Boundaries");
    snprintf(memoryArray[memIndex].data, 64, "%d %d", newProcess.Memoryboundaries.min, newProcess.Memoryboundaries.max);
    int boundariesMax= memIndex;
    memIndex++;
    memIndex++;
    memIndex++;
    memIndex++;

    int PC = memIndex;
    newProcess.PC = PC;


    char line[64];

    while (fgets(line, sizeof(line), file)) {
        snprintf(memoryArray[memIndex].name, 64 , "Instruction");
        snprintf(memoryArray[memIndex].data, 64, line);
        memIndex++;
    }

    newProcess.Memoryboundaries.max = memIndex-1;
    snprintf(memoryArray[boundariesMax].name, 64 , "Boundaries");
    snprintf(memoryArray[boundariesMax].data, 64, "%d %d", newProcess.Memoryboundaries.min, newProcess.Memoryboundaries.max);
    snprintf(memoryArray[PCindex].name, 64 , "PC");
    snprintf(memoryArray[PCindex].data, 64, "%d", PC);
    memIndex = newProcess.Memoryboundaries.max + 1;
    programs[processID] = newProcess;

    printf("\n");
    int min, max;
    sscanf(memoryArray[boundariesMax].data, "%d %d", &min, &max);
    printf("Min: %d\n", min);
    printf("Max: %d\n", max);
    inst++;
    fclose(file);

}

int main(){
    initializeMemoryArray();
    addProcess(1, 0,"Program_1.txt");
    addProcess(2, 0,"Program_2.txt");
    addProcess(3, 0,"Program_3.txt");
    printMemoryArray();
    return 0;
}