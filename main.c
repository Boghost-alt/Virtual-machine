#include <stddef.h>
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
} Inst;

Inst parse_inst(char *line);

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
    while(str[*currentPos] == ' ')
    {
        (*currentPos)++;
    }
}

void push(int val, const char* reg)
{
    if(strcmp(reg, "stack") == 0 && stack_size < MAX_STACK_SIZE)
        stack[stack_size++] = val;
    else if(strcmp(reg, "ax") == 0 && a_size < MAX_REG_SIZE)
        ax[a_size++] = val;
    else if(strcmp(reg, "bx") == 0 && b_size < MAX_REG_SIZE)
        bx[b_size++] = val;
    else if(strcmp(reg, "cx") == 0 && c_size < MAX_REG_SIZE)
        cx[c_size++] = val;
    else
        printf("ERROR: Stack overflow\n");
}

int pop(const char* reg)
{
    if(strcmp(reg, "stack") == 0 && stack_size > 0)
        return stack[--stack_size];
    else if(strcmp(reg, "ax") == 0 && a_size > 0)
        return ax[--a_size];
    else if(strcmp(reg, "bx") == 0 && b_size > 0)
        return bx[--b_size];
    else if(strcmp(reg, "cx") == 0 && c_size > 0)
        return cx[--c_size];
    else
    {
        printf("ERROR: Stack underflow\n");
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
            printf("ERROR: cx register overflow!\n");
    }
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
    if(strcmp(reg, "ax") == 0)
    {
        printf("ax: \n");
        for(int i = a_size - 1; i >= 0; i--)  
            printf("%d\n", ax[i]);  
    }else if(strcmp(reg, "bx") == 0)
    {
        printf("bx: \n");
        for(int i = b_size - 1; i >= 0; i--)  
            printf("%d\n", bx[i]);  
    }else if(strcmp(reg, "cx") == 0)
    {
        printf("cx:  \n");
        for(int i = c_size - 1; i >= 0; i--)  
            printf("%d\n", cx[i]);  
    }else if(strcmp(reg, "stack") == 0)
    {
        printf("stack:  \n");
        for(int i = stack_size - 1; i >= 0; i--)  
            printf("%d\n", stack[i]);  
    }
}

Inst parse_inst(char *line)
{
    Inst inst;
    char buff[BUFSIZ];
    size_t i = 0;

    skipspace(line, &i);

    // Get the instruction type
    size_t j = 0;
    while(line[i] != ' ' && line[i] != '\0') {
        buff[j++] = line[i++];
    }
    buff[j] = '\0';

    if (strcmp(buff, "PUSH") == 0)
        inst.type = Inst_PUSH;
    else if (strcmp(buff, "POP") == 0)
        inst.type = Inst_POP;
    else if (strcmp(buff, "ADD") == 0)
        inst.type = Inst_ADD;
    else if (strcmp(buff, "SUB") == 0)
        inst.type = Inst_SUB;
    else if (strcmp(buff, "MOV") == 0)
        inst.type = Inst_MOV;
    else {
        printf("ERROR: Unknown instruction %s\n", buff);
        exit(1);
    }

    skipspace(line, &i); // Skip space after instruction

    j = 0;
    while(line[i] != ' ' && line[i] != '\0') {
        buff[j++] = line[i++];
    }
    buff[j] = '\0';

    if(inst.type == Inst_PUSH || inst.type == Inst_MOV)
        inst.value = atoi(buff);
    else
        inst.value = 0;

    skipspace(line, &i); // Skip space after value

    j = 0;
    while(line[i] != '\0') {
        buff[j++] = line[i++];
    }
    buff[j] = '\0';

    if (strcmp(buff, "") == 0)
        inst.reg = "stack";
    else if (strcmp(buff, "ax") == 0)
        inst.reg = "ax";
    else if (strcmp(buff, "bx") == 0)
        inst.reg = "bx";
    else if (strcmp(buff, "cx") == 0)
        inst.reg = "cx";
    else {
        printf("ERROR: Unknown register %s\n", buff);
        exit(1);
    }

    return inst;
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("Incorrect usage, expected: ./main file.txt\n");
        exit(1);
    }
    
    FILE *fp = fopen(argv[1], "r");
    if(fp == NULL)
    {
        fprintf(stderr, "ERROR: Unable to open file\n");
        exit(1);
    }
    
    char buff[BUFSIZ];
    while(fgets(buff, BUFSIZ, fp) != NULL)
    {
        size_t len = strlen(buff);
        if(buff[len - 1] == '\n')
            buff[len - 1] = '\0';
        
        Inst inst = parse_inst(buff);
        int a, b;
        switch(inst.type)
        {
            case Inst_PUSH:
                push(inst.value, inst.reg);
                break;
            case Inst_POP:
                a = pop(inst.reg);
                break;
            case Inst_ADD:
                a = pop(inst.reg);
                b = pop(inst.reg);
                push(a + b, inst.reg);
                break;
            case Inst_SUB:
                a = pop(inst.reg);
                b = pop(inst.reg);   
                push(a - b, inst.reg);
                break;
            case Inst_MOV:
                mov(inst.value, inst.reg);
                break;
        }
    }

    print_register("ax");
    print_register("bx");
    print_register("cx");
    print_register("stack");
    
    fclose(fp);
    
    return 0;
}
