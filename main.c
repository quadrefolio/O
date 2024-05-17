#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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





void translate(){
    
}



int main(){
    PCB b ;
    b.Memoryboundaries.max=10;
    char hi [32]="hello";
    printf("hi %d",b.Memoryboundaries.max);
    return 0;
}