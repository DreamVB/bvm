#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_STACK 1000
#define MAX_CODE 2048
#define MAX_REGS 8

enum TOps{
    HLT = 0,
    PUSH,
    POP,
    ADD,
    SUB,
    MUL,
    DIV,
    AND,
    OR,
    XOR,
    MOD,
    STORE,
    LOAD,
    PRINT_I,
    PRINT_C,
}op_codes;

int regs[MAX_REGS];
int STACK[MAX_STACK];
int p_code[MAX_CODE];
int p_code_size = 0;

int pc = 0;
int sp = -1;

bool running = true;

void vm_init(int *source, int length){
    int i = 0;

    while(i < length){
        p_code[i] = source[i];
        p_code_size++;
        i++;
    }
}

void vm_execute(){
int a,b;
int addr = 0;

    while(running){
        op_codes = p_code[pc];

        switch(op_codes){
            case HLT:
                running = false;
                break;
            case PUSH:
                pc++;
                STACK[++sp] = p_code[pc];
                break;
            case POP:
                STACK[sp--];
                break;
            case STORE:
                pc++;
                addr = p_code[pc];
                regs[addr] = STACK[sp--];
                break;
            case LOAD:
                pc++;
                addr = p_code[pc];
                STACK[++sp] = regs[addr];
            case ADD:
                b = STACK[sp--];
                a = STACK[sp--];
                STACK[++sp] = a + b;
                break;
            case SUB:
                b = STACK[sp--];
                a = STACK[sp--];
                STACK[++sp] = a - b;
                break;
            case MUL:
                b = STACK[sp--];
                a = STACK[sp--];
                STACK[++sp] = a * b;
                break;
            case DIV:
                b = STACK[sp--];
                a = STACK[sp--];

                if(b == 0){
                    printf("Cannot Divide By Zero.\n");
                    running = false;
                    return;
                }

                STACK[++sp] = a / b;
                break;
            case AND:
                b = STACK[sp--];
                a = STACK[sp--];
                STACK[++sp] = a & b;
                break;
            case OR:
                b = STACK[sp--];
                a = STACK[sp--];
                STACK[++sp] = a & b;
                break;
            case XOR:
                b = STACK[sp--];
                a = STACK[sp--];
                STACK[++sp] = a ^ b;
                break;
            case MOD:
                b = STACK[sp--];
                a = STACK[sp--];
                STACK[++sp] = a % b;
                break;
            case PRINT_I:
                printf("%d\n",STACK[sp--]);
                break;
            case PRINT_C:
                printf("%c",STACK[sp--]);
        }

        pc++;
    }
}

void EmitCode(int source){
    p_code[p_code_size] = source;
    p_code_size++;
}

int main(int argc, char **argv)
{
    FILE *fp = NULL;
    int x = 0;
    int bCode = 0;

    /*Uncomment for testing
    int len = sizeof(var_test) / sizeof(int);
    vm_init(var_test,len);
    vm_execute();
    */

    if(argc <= 1){
        printf("Missing Input.\n");
        printf("Usage: ./bvm program_name.vm\n");
        printf("Error Code(%d)\n",1);
        return EXIT_FAILURE;
    }

    fp = fopen(argv[1],"rb");
    //Check if input file was opened.
    if(!fp){
        printf("Error Reading Input.\n");
        printf("Error Code(%d)\n",2);
        return EXIT_FAILURE;
    }
    //Load the program in to the p_code array.

    while(!feof(fp)){
        bCode = getw(fp);
        if(bCode != EOF){
            EmitCode(bCode);
        }
    }
    //Close input file.
    fclose(fp);
    //Execute the program code.
    vm_execute();

    return EXIT_SUCCESS;
}
