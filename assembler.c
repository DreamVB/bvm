#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_VARS 26
#define MAX_CODE 2048
#define MAX_TOKEN 21
#define MAX_PARMS 80
#define MAX_LINE 255

char strLine[MAX_LINE];
char token[MAX_TOKEN];
char parms[MAX_PARMS];
char vars[MAX_VARS];

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

int p_code[MAX_CODE];
int p_code_size = 0;

int _getvar_index(char c);

void Emit(int source){
    p_code[p_code_size] = source;
    p_code_size++;
}

int _chrpos(char *source, char c){
    int x = 0;
    int pos =-1;
    while(x < strlen(source)){
        if(source[x] == c){
            pos = x;
            break;
        }
        x++;
    }
    return pos;
}

void trim(char * s) {
    char * p = s;
    int l = strlen(p);

    while(isspace(p[l - 1])) p[--l] = 0;
    while(* p && isspace(* p)) ++p, --l;

    memmove(s, p, l + 1);
}


void GetParms(char *source, int start, int tend){
int x = start + 1;
int y  = 0;

    while(x < strlen(source)){
        parms[y] = source[x];
        x++;
        y++;
    }
    parms[y] = '\0';
}

void UCase(char *source){
    int x = 0;
    while(x < strlen(source)){
        source[x] = toupper(source[x]);
        x++;
    }
}

bool is_number(char *source){
    int x = 0;
    bool isNum = true;

    while(x < strlen(source)){
        if(!isdigit(source[x])){
            isNum = false;
            break;
        }
        x++;
    }
    return isNum;
}

void GetToken(char *source, int start, int tend){
int x = start;
int y  = 0;

    while(x < tend){
        if(!isspace(source[x])){
            token[x] = source[y];
            y++;
        }
        x++;
    }
    token[y] = '\0';
}

void pgm_parser(){
int i_tmp = 0;
int v_idx = 0;
char c_tmp = '\0';

    //Process tokens
    //PUSH
    if(strcmp(token,"PUSH") == 0){
        //Emit byte code for push
        Emit(PUSH);
        //Check for number value
        if(is_number(parms)){
            i_tmp = atoi(parms);
            //Add byte code.
            Emit(i_tmp);
        }else{
            //Convert the char to a int
            c_tmp = parms[1];
            //Must be a char.
            Emit(c_tmp);
        }
    }
    //STORE
    if(strcmp(token,"STORE") == 0){
        Emit(STORE);
        //Get var
        c_tmp = toupper(parms[0]);
        //Get variable index
        Emit(c_tmp);
    }

    //STORE
    if(strcmp(token,"LOAD") == 0){
        Emit(LOAD);
        //Get var
        c_tmp = toupper(parms[0]);
        //Get variable index
        Emit(c_tmp);
    }

    //PRINT_I
    if(strcmp(token,"PRINT_I") == 0){
        //Emit byte code
        Emit(PRINT_I);
    }
    //PRINT_C
    if(strcmp(token,"PRINT_C") == 0){
        Emit(PRINT_C);
    }

    if(strcmp(token,"ADD") == 0){
        //Emit byte code
        Emit(ADD);
    }
    if(strcmp(token,"HLT") == 0){
        //Emit byte code
        Emit(HLT);
    }
    if(strcmp(token,"AND") == 0){
        Emit(ADD);
    }
    if(strcmp(token,"OR") == 0){
        Emit(OR);
    }
    if(strcmp(token,"XOR") == 0){
        Emit(XOR);
    }
    if(strcmp(token,"MOD") == 0){
        Emit(MOD);
    }
}

void _loadvars(){
    int x  = 0;

    while(x < MAX_VARS){
        vars[x] = (char)'A' + x;
        x++;
    }
}

int _getvar_index(char c){
    int x = 0;

    while(x < MAX_VARS){
        if(c == vars[x]){
            return x;
        }
        x++;
    }
    return -1;
}

int main(int argc, char **argv)
{
    FILE *fp = NULL;
    FILE *f_out = NULL;

    int ch_pos = -1;
    int len = 0;
    int cnt = 0;

    //Load vars
    _loadvars();

    if(argc != 3){
        printf("Missing Program File.\n");
        printf("Usage: basm Hello.basm Hello.vm\n");
        printf("Error Code(%d)\n",1);
        return EXIT_FAILURE;
    }

    //Open file
    fp = fopen(argv[1],"rb");

    if(!fp){
        printf("Error Reading Source File\n%s\n",argv[1]);
        printf("Error Code(%d)\n",2);
        return EXIT_FAILURE;
    }

    //Read and process source code.
    while(fgets(strLine,MAX_LINE,fp) != NULL){
        //Check for comments
        if(strLine[0] != ';'){
            if(strlen(strLine) > 2){
                //Next we need to split the line into token and value
                ch_pos = _chrpos(strLine,' ');
                len = strlen(strLine);

                if(ch_pos != -1){
                    GetToken(strLine,0,ch_pos);
                    GetParms(strLine,ch_pos,len);
                    trim(parms);
                }else{
                    //Get token
                    GetToken(strLine,0,len);
                }
                //Process tokens.
                if(strlen(token) > 0){
                    UCase(token);
                    pgm_parser();
                }
                //
            }
        }
    }
    //Close file.
    fclose(fp);

    //Write out object code.
    f_out = fopen(argv[2],"wb");

    if(!f_out){
        printf("Error Write Object File.\n");
        printf("Error Code(%d)\n",3);
        return EXIT_FAILURE;
    }else{
        //Write data in p_code to the output file.
        while(cnt < p_code_size){
            //write out int to file.
            putw(p_code[cnt],f_out);
            //INC counter
            cnt++;
        }
    }
    //Close output file.
    fclose(f_out);

    return EXIT_SUCCESS;
}
