#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 4096
#define MAX_REG_SIZE 4096
#define PROGRAM_SIZE (sizeof(instructions)/sizeof(instructions[0]))

#define PUSH(x, y) {.reg=x, .value=y, .type=Inst_PUSH}
#define POP(x) {.reg=x, .type=Inst_POP}
#define ADD(x) {.reg=x, .type=Inst_ADD}
#define SUB(x) {.reg=x, .type=Inst_SUB}
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

Inst instructions[] = {
    PUSH("stack", 15),
    PUSH("ax", 12),
    ADD("ax"),
    MOV("ax", 2),
    MOV("stack", 0),
    PUSH("ax", 10),
    SUB("ax"),
};

void skipspace(const char* str, size_t *currentPos)
{
    while(str[*currentPos]==' ')
    {
        (*currentPos)++;
    }
}

void push(int val, const char* reg)
{
    if(strcmp(reg, "stack") && stack_size < MAX_STACK_SIZE)
        stack[stack_size++] = val;
    else if(strcmp(reg, "ax") && a_size < MAX_REG_SIZE)
        ax[a_size++] = val;
    else if(strcmp(reg, "bx") && b_size < MAX_REG_SIZE)
        bx[b_size++] = val;
    else if(strcmp(reg, "cx")&& c_size < MAX_REG_SIZE)
        cx[c_size++] = val;
    else
        printf("ERROR : Stack overflow");
}

int pop(const char* reg)
{
    if(strcmp(reg, "stack") && stack_size > 0)
        return stack[--stack_size];
    else if(strcmp(reg, "ax") && a_size > 0)
        return ax[--a_size];
    else if(strcmp(reg, "bx") && b_size > 0)
        return bx[--b_size];
    else if(strcmp(reg, "cx")&& c_size > 0)
        return cx[--c_size];
    else
    {
        printf("ERROR : Stack underflow");
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
    }
    else if(strcmp(reg, "bx") == 0)
    {
        if(b_size < MAX_REG_SIZE)
            bx[b_size++] = val;
        else
            printf("ERROR: bx register overflow!\n");
    }
    else if(strcmp(reg, "cx") == 0)
    {
        if(c_size < MAX_REG_SIZE)
            cx[c_size++] = val;
        else

    else if(strcmp(reg, "stack") == 0)
    {
        if(stack_size < MAX_STACK_SIZE)
            stack[stack_size++] = val;
        else
            printf("ERROR: Stack overflow!\n");
    }
}

void print_register(char* reg)
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
                push(instructions[i].value, instructions[i].reg);
                break;
            case Inst_POP:
                a = pop(instructions[i].reg);
                break;
            case Inst_ADD:
                a = pop(instructions[i].reg);
                b = pop(instructions[i].reg);
                push(a + b, instructions[i].reg);
                break;
            case Inst_SUB:
                a = pop(instructions[i].reg);
                b = pop(instructions[i].reg);   
                push(a-b,instructions[i].reg);
                break;
            case Inst_MOV:
                mov(instructions[i].value, instructions[i].reg);
                break;
                
        }
    }
    print_register("ax");
    print_register("bx");
    print_register("cx");
    print_register("stack");
    return 0;
}

Inst read(char* filePath, int targetLine)
{
    FILE* fp = fopen(filePath, "r");
    char buff1[BUFSIZ], buff2[BUFSIZ];
    size_t currentLine = 0;
    Inst instruction;
    size_t i, j;
    
    while(fgets(buff1, sizeof(buff1), fp))
    {
        currentLine++;
        if(currentLine == targetLine)
        {
            while(buff1[i] !=' ' && buff1[i] != '\0')
            {
                buff2[j++] = buff1[i++];
            }
            if(strcmp(buff2, "PUSH"))
                instruction.type = Inst_PUSH;
            else if(strcmp(buff2, "POP"))
                instruction.type = Inst_POP;
            else if(strcmp(buff2, "ADD"))
                instruction.type = Inst_ADD;
            else if(strcmp(buff2, "SUB"))
                instruction.type = Inst_SUB;
            else if(strcmp(buff2, "MOV"))
                instruction.type = Inst_MOV;
            j=0;
            memset(buff2, 0, BUFSIZ);
            skipspace(buff1, &i);
            while(buff1[i] != ' ' && buff1[i] != '\0')
                buff2[j++] = buff1[i++];
            instruction.value = atoi(buff2);
            j=0;
            memset(buff2, 0, BUFSIZ);
            skipspace(buff1, &i);
            while(buff1[i] != ' ' && buff1[i] != '\0')
                buff2[j++] = buff1[i++];
            if(strcmp(buff2, ""))
                instruction.reg = "stack";
            else if(strcmp(buff2, "ax"))
                instruction.reg = "ax";
            else if(strcmp(buff2, "bx"))
                instruction.reg = "bx";
            else if(strcmp(buff2, "cx"))
                instruction.reg = "cx";
        }
    }
    
    return instruction;
}

