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
}Inst;

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

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("Incorrect usage, expected : ./main \"file.txt\"");
        exit(1);
    }
    
    FILE *fp = fopen(argv[1], "r");
    if(fp==NULL)
    {
        fprintf(stderr, "ERROR : Unable to open file");
        exit(1);
    }
    stack_size = 0; // Explicit initialization

    int a, b;
    char buff[BUFSIZ];
    
    while(fgets(buff, BUFSIZ, fp) != NULL)
    {
        size_t len = strlen(buff);
        if(buff[len-1] == '\n')
            buff[len-1] = '\0';
        
        Inst inst = parse_inst(buff);
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
                push(a-b,inst.reg);
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

Inst parse_int(char *line)
{
    Inst inst;
    char buff[BUFSIZ];
    size_t i;
    
    skipspace(line, &i);
    
    size_t j=0;
    while(line[i] != ' ' && line[i] != '\0')
        buff[j++] = line[i++];
    buff[j] = '\0';
    

    if(strcmp(buff, "PUSH"))
        inst.type = Inst_PUSH;
    else if(strcmp(buff, "POP"))
        inst.type = Inst_POP;
    else if(strcmp(buff, "ADD"))
        inst.type = Inst_ADD;
    else if(strcmp(buff, "SUB"))
        inst.type = Inst_SUB;
    else if(strcmp(buff, "MOV"))
        inst.type = Inst_MOV;

    skipspace(line, &i);
    j=0;
    while(line[i] != '\0')
        buff[j++] = line[i++];
    buff[j] = '\0';
    
    if(inst.type == Inst_PUSH)
        inst.value = atoi(buff);
    else
        inst.value = 0;
    
    skipspace(line, &i);
    j=0;
    while(line[i] != '\0')
        buff[j++] = line[i++];
    buff[j] = '\0';
    
    if(strcmp(buff, ""))
        inst.reg = "stack";
    else if(strcmp(buff, "ax"))
        inst.reg = "ax";
    else if(strcmp(buff, "bx"))
        inst.reg = "bx";
    else if(strcmp(buff, "cx"))
        inst.reg = "cx";
    
    return inst;
}

