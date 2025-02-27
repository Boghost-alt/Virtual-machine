#include <stdio.h>

#define MAX_STACK_SIZE 4096

#define PUSH(x) {.value=x, .type=Inst_PUSH}
#define POP {.type=Inst_POP}
#define ADD {.type=Inst_ADD}

typedef enum{
    Inst_PUSH,
    Inst_POP,
    Inst_ADD,
}Inst_set;

typedef struct{
    int value;
    Inst_set type;
}Inst;

int stack[MAX_STACK_SIZE] = {0,};
int stack_size;

void push(int a)
{
    stack[stack_size++] = a;
}

int pop()
{
    return stack[--stack_size];
}

int main()
{
    printf("Hello world\n");
    return 0;
}