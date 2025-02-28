#include <stdio.h>
#include <string.h>

#define MAX_STACK_SIZE 4096
#define MAX_REG_SIZE 4096
#define PROGRAM_SIZE (sizeof(instructions)/sizeof(instructions[0]))

#define PUSH(x) {.value=x, .type=Inst_PUSH}
#define POP {.type=Inst_POP}
#define ADD {.type=Inst_ADD}
#define SUB {.type=Inst_SUB}
#define MOV(x, y) {.reg=x, .value=y, .type=Inst_MOV}

typedef enum{
    Inst_PUSH,
    Inst_POP,
    Inst_ADD,
    Inst_SUB,
    Inst_MOV,
} Inst_set;

typedef struct{
    const char *reg;
    int value;
    Inst_set type;
}Inst;

int stack[MAX_STACK_SIZE] = {0,};
int stack_size = 0;

int ax[MAX_REG_SIZE] = {0,};
int a_size = 0;
int bx[MAX_REG_SIZE] = {0,};
int b_size = 0;
int cx[MAX_REG_SIZE] = {0,};
int c_size = 0;

int *current_reg = stack;

Inst instructions[] = {
    PUSH(15),
    PUSH(12),
    ADD,
    MOV("ax", 2),
    MOV("stack", 0),
    PUSH(10),
    SUB,
};

void push(int val)
{
    if(current_reg==stack && stack_size < MAX_STACK_SIZE)
        stack[stack_size++] = val;
    else if(current_reg==ax && a_size < MAX_REG_SIZE)
        ax[a_size++] = val;
    else if(current_reg==bx && b_size < MAX_REG_SIZE)
        bx[b_size++] = val;
    else if(current_reg==cx && c_size < MAX_REG_SIZE)
        cx[c_size++] = val;
    else
        printf("ERROR : Stack overflow");
}

int pop()
{
    if(current_reg==stack && stack_size > 0)
        return stack[--stack_size];
    else if(current_reg==ax && a_size > 0)
        return ax[--a_size];
    else if(current_reg==bx && b_size > 0)
        return bx[--b_size];
    else if(current_reg==cx && c_size > 0)
        return cx[--c_size];
    else if(current_reg==stack && stack_size > 0)
        return stack[--stack_size];
    else{
        printf("ERROR: Stack underflow!\n");
        return 0;
    }
}

void mov(int val, const char* reg)
{
    if(strcmp(reg, "ax") == 0)
    {
        if(a_size < MAX_REG_SIZE)
            ax[a_size++] = val;
        else
            printf("ERROR: ax register overflow!\n");
        current_reg = ax;
    }
    else if(strcmp(reg, "bx") == 0)
    {
        if(b_size < MAX_REG_SIZE)
            bx[b_size++] = val;
        else
            printf("ERROR: bx register overflow!\n");
        current_reg = bx;
    }
    else if(strcmp(reg, "cx") == 0)
    {
        if(c_size < MAX_REG_SIZE)
            cx[c_size++] = val;
        else
            printf("ERROR: cx register overflow!\n");
        current_reg = cx;
    }
    else if(strcmp(reg, "stack") == 0)
    {
        if(stack_size < MAX_STACK_SIZE)
            stack[stack_size++] = val;
        else
            printf("ERROR: Stack overflow!\n");
        current_reg = stack;
    }
}

void print_reg(char* reg)
{
    if(strcmp(reg, "ax")==0)
    {
        printf("ax : \n");
        for(int i = a_size - 1; i >= 0; i--)  
            printf("%d\n", ax[i]);  
    }else if(strcmp(reg, "bx")==0)
    {
        printf("bx : \n");
        for(int i = b_size - 1; i >= 0; i--)  
            printf("%d\n", bx[i]);  
    }else if(strcmp(reg, "cx")==0)
    {
        printf("cx :  \n");
        for(int i = c_size - 1; i >= 0; i--)  
            printf("%d\n", cx[i]);  
    }else if(strcmp(reg, "stack")==0)
    {
        printf("stack :  \n");
        for(int i = stack_size - 1; i >= 0; i--)  
            printf("%d\n", stack[i]);  
    }

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
            case Inst_SUB:
                a = pop();
                b = pop();
                push(a-b);
                break;
            case Inst_MOV:
                mov(instructions[i].value, instructions[i].reg);
                break;
                
        }
    }
    print_reg("ax");
    print_reg("bx");
    print_reg("cx");
    print_reg("stack");
    return 0;
}
