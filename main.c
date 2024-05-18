
#include <stdio.h>
#include <string.h>
#include <stdlib.h>





MEMORY_SIZE = 61;
Memory memoryArray[MEMORY_SIZE];


typedef struct 
{
    int min;
    int max;
    
}MemoryBoundaries;


typedef struct 
{
    int Pid;
    char Pstate [32];
    int Priority;
    int PC;
    MemoryBoundaries Memoryboundaries;
}PCB;


typedef struct 
{
    char name [32];
    char data [64];
}Memory;


void initializeMemoryArray() {

    for (int i = 0; i < MEMORY_SIZE; i++) {
        strcpy(memoryArray[i].name, " ");
        strcpy(memoryArray[i].data, " ");
    }
}






void translate(){
    
    
}




int main(){
    initializeMemoryArray();
    
    return 0;
}