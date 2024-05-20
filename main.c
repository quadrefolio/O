
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MEMORY_SIZE 61
#define NUM_INST 3


struct Node {
    int data;
    struct Node* next;
};

// Function to create a new node
struct Node* newNode(int data) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->data = data;
    node->next = NULL;
    return node;
}

// Queue structure
struct Queue {
    struct Node *front, *rear;
};

// Function to create a new empty queue
struct Queue* createQueue() {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->front = queue->rear = NULL;
    return queue;
}

// Function to add an element to the queue
void enqueue(struct Queue* queue, int data) {
    struct Node* node = newNode(data);
    // If the queue is empty, new node becomes both front and rear
    if (queue->rear == NULL) {
        queue->front = queue->rear = node;
        return;
    }
    // Add the new node at the end of the queue and update rear
    queue->rear->next = node;
    queue->rear = node;
}

// Function to remove an element from the queue
int dequeue(struct Queue* queue) {
    // If the queue is empty, return -1
    if (queue->front == NULL)
        return -1;

    // Store the current front and move the front one node ahead
    struct Node* temp = queue->front;
    queue->front = queue->front->next;

    // If the front becomes NULL, then the queue is empty
    if (queue->front == NULL)
        queue->rear = NULL;

    int data = temp->data;
    free(temp);
    return data;
}

// Function to print all elements of the queue
void printQueue(struct Queue* queue) {
    struct Node* temp = queue->front;
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}


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
    int var;
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
        return ;
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
    int var = boundariesMax +1;
    newProcess.var = var;
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
    inst++;
    fclose(file);

}

void executeInstruction(int processID) {
    PCB *process = &programs[processID];
    int PC = process->PC +1;

    if (PC < process->Memoryboundaries.min || PC > process->Memoryboundaries.max) {
        printf("Error: Program Counter out of bounds.\n");
        return;
    }

    char *instruction = memoryArray[PC].data;
    printf("Executing instruction: %s\n", instruction);

    // Parse the instruction
    char command[32];
    sscanf(instruction, "%s", command);

    if (strcmp(command, "print") == 0) {
        char var[32];
        sscanf(instruction, "print %s", var);
        for (int i = process->Memoryboundaries.min; i <= process->Memoryboundaries.max; i++) {
            if (strcmp(memoryArray[i].name, var) == 0) {
                printf("Output: %s\n", memoryArray[i].data);
                break;
            }
        }
    } 
    else if (strcmp(command, "assign") == 0) {
        char var[32], value[32];
        sscanf(instruction, "assign %s %s", var, value);
        strcpy(memoryArray[programs[processID].var].name, var);
        strcpy(memoryArray[programs[processID].var].data, value);
        process->var++;
        
    } 
    else if (strcmp(command, "writeFile") == 0) {
        char filename[32], data[64];
        sscanf(instruction, "writeFile %s %s", filename, data);
        FILE *file = fopen(filename, "w");
        if (file) {
            fprintf(file, "%s", data);
            fclose(file);
        } else {
            perror("Error opening file");
        }
    } 
    else if (strcmp(command, "readFile") == 0) {
        char filename[32];
        sscanf(instruction, "readFile %s", filename);
        FILE *file = fopen(filename, "r");
        if (file) {
            char line[64];
            while (fgets(line, sizeof(line), file)) {
                printf("%s", line);
            }
            fclose(file);
        } else {
            perror("Error opening file");
        }
    } 
    else if (strcmp(command, "printFromTo") == 0) {
        int x, y;
        sscanf(instruction, "printFromTo %d %d", &x, &y);
        for (int i = x; i <= y; i++) {
            printf("%d\n", i);
        }
    } 
    else if (strcmp(command, "semWait") == 0) {
        char resource[32];
        sscanf(instruction, "semWait %s", resource);
        if (strcmp(resource, "userInput") == 0) {
            printf("Waiting for user input...\n");
        } else if (strcmp(resource, "userOutput") == 0) {
            printf("Waiting for user output...\n");
        } else if (strcmp(resource, "file") == 0) {
            printf("Waiting for file access...\n");
        }
    } 
    else if (strcmp(command, "semSignal") == 0) {
        char resource[32];
        sscanf(instruction, "semSignal %s", resource);
        if (strcmp(resource, "userInput") == 0) {
            printf("Signaling user input...\n");
        } else if (strcmp(resource, "userOutput") == 0) {
            printf("Signaling user output...\n");
        } else if (strcmp(resource, "file") == 0) {
            printf("Signaling file access...\n");
        }
    }
}

int main(){
    initializeMemoryArray();
    addProcess(1, 0,"Program_1.txt");
    addProcess(2, 0,"Program_2.txt");
    addProcess(3, 0,"Program_3.txt");
 
    executeInstruction(2);
    programs[2].PC ++;
    executeInstruction(2);
    programs[2].PC ++;
    executeInstruction(2);
    
    printMemoryArray();
    return 0;
}