#include <stdio.h>

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

int main()
{
    printf("Hello world\n");
    return 0;
}