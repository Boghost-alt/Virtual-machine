#include <stdio.h>

#define MAX_STACK_SIZE 4096
#define PROGRAM_SIZE (sizeof(instructions)/sizeof(instructions[0]))

#define PUSH(x) {.value=x, .type=Inst_PUSH}
#define POP {.type=Inst_POP}
#define ADD {.type=Inst_ADD}

typedef enum{
    Inst_PUSH,
    Inst_POP,
    Inst_ADD,
} Inst_set;

typedef struct{
    int value;
    Inst_set type;
} Inst;

int stack[MAX_STACK_SIZE] = {0,};
int stack_size = 0;

Inst instructions[] = {
    PUSH(15),
    PUSH(12),
    ADD,
};

void push(int a)
{
    stack[stack_size++] = a;
}

int pop()
{
    if (stack_size == 0)
    {
        printf("Error: Stack underflow!\n");
        return 0;
    }
    return stack[--stack_size];
}

void print_stack()
{
    for(int i = stack_size - 1; i >= 0; i--)  
        printf("%d\n", stack[i]);  
}

int main()
{
    stack_size = 0; // Explicit initialization

    int a, b;
    for(int i = 0; i < PROGRAM_SIZE; i++)  // Fix loop condition
    {
        switch(instructions[i].type)
        {
            case Inst_PUSH:
                push(instructions[i].value);
                break;
            case Inst_POP:
                a = pop();
                break;
            case Inst_ADD:
                a = pop();
                b = pop();
                push(a + b);
                break;
        }
    }
    print_stack();
    return 0;
}
