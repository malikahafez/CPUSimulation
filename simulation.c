#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//clock cycle counter
int cycles = 0;

//instruction memory
short int instMem[1024];//16-bit inst
int numInst = sizeof(instMem)/sizeof(short int);
int instPtr = 0;
//0b0000000000000000

//data memory
int8_t dataMem[2048];//8-bit data
//0b00000000

//register file (64 GPRs)
int8_t regFile[64];//8-bit registers
//0b00000000

//status reg
int8_t SREG;//8-bit status register
//0b000CVNSZ
//0b00000000

//program counter
short int pc = 0;//16 bit pc
//0b0000000000000000

//function to parse the instructions from the txt file and stores them in the instMem
void parseandStore(char* filename) {
    short int parsed = 0;//instruction in binary
    short int opcode = 0;//both I and R
    short int r1 = 0;//both I and R
    short int r2 = 0;//R
    short int immediate = 0;//I

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file: %s\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char temp[256];
        strcpy(temp, line);
        char *command = strtok(temp, " \n");
        if (command == NULL) break;
        if (strcmp(command, "ADD") == 0) {
            opcode = 0;
            opcode = opcode<<12;//put opcode in first 4 bits
            char *R1 = strtok(NULL, " \r\n");//R1
            char *R2 = strtok(NULL, " \r\n");//R2
                r1 = (short int) atoi(R1 + 1); // R1 -> 1
                r1 = r1<<6;

                r2 = (short int) atoi(R2 + 1); // R1 -> 1

            parsed = opcode | r1 | r2;
        }
        else if (strcmp(command, "SUB") == 0) {
            opcode = 1;
            opcode = opcode<<12;//put opcode in first 4 bits

            char *R1 = strtok(NULL, " \r\n");//R1
            char *R2 = strtok(NULL, " \r\n");//R2
                r1 = (short int) atoi(R1 + 1); // R1 -> 1
                r1 = r1<<6;
                r2 = (short int) atoi(R2 + 1); // R1 -> 1

            parsed = opcode | r1 | r2;

        }
        else if (strcmp(command, "MUL") == 0) {
            opcode = 2;
            opcode = opcode<<12;//put opcode in first 4 bits

            char *R1 = strtok(NULL, " \r\n");//R1
            char *R2 = strtok(NULL, " \r\n");//R2
                r1 = (short int) atoi(R1 + 1); // R1 -> 1
                r1 = r1<<6;
                r2 = (short int) atoi(R2 + 1); // R1 -> 1

            parsed = opcode | r1 | r2;

        }
        else if (strcmp(command, "MOVI") == 0) {
            opcode = 3;
            opcode = opcode<<12;//put opcode in first 4 bits

            char *im = strtok(NULL, " \r\n");//IMM
            char *R1 = strtok(NULL, " \r\n");//R1
                r1 = (short int) atoi(R1 + 1); // R1 -> 1
                r1 = r1<<6;


            immediate = (short int) atoi(im + 1); 
            parsed = opcode | r1 | immediate;

        }
        else if (strcmp(command, "BEQZ") == 0) {
            opcode = 4;
            opcode = opcode<<12;//put opcode in first 4 bits

            char *im = strtok(NULL, " \r\n");//IMM
            char *R1 = strtok(NULL, " \r\n");//R1
                r1 = (short int) atoi(R1 + 1); // R1 -> 1
                r1 = r1<<6;

            immediate = (short int) atoi(im + 1); 
            parsed = opcode | r1 | immediate;

        }
        else if (strcmp(command, "ANDI") == 0) {
            opcode = 5;
            opcode = opcode<<12;//put opcode in first 4 bits

            char *im = strtok(NULL, " \r\n");//IMM
            char *R1 = strtok(NULL, " \r\n");//R1
                r1 = (short int) atoi(R1 + 1); // R1 -> 1
                r1 = r1<<6;

            immediate = (short int) atoi(im + 1); 
            parsed = opcode | r1 | immediate;

        }
        else if (strcmp(command, "EOR") == 0) {
            opcode = 6;
            opcode = opcode<<12;//put opcode in first 4 bits

            char *R1 = strtok(NULL, " \r\n");//R1
            char *R2 = strtok(NULL, " \r\n");//R2
                r1 = (short int) atoi(R1 + 1); // R1 -> 1
                r1 = r1<<6;

                r2 = (short int) atoi(R2 + 1); // R1 -> 1

            parsed = opcode | r1 | r2;

        }
        else if (strcmp(command, "BR") == 0) {
            opcode = 7;
            opcode = opcode<<12;//put opcode in first 4 bits

            char *R1 = strtok(NULL, " \r\n");//R1
            char *R2 = strtok(NULL, " \r\n");//R2
                r1 = (short int) atoi(R1 + 1); // R1 -> 1
                r1 = r1<<6;

                r2 = (short int) atoi(R2 + 1); // R1 -> 1

            parsed = opcode | r1 | r2;

        }
        else if (strcmp(command, "SAL") == 0) {
            opcode = 8;
            opcode = opcode<<12;//put opcode in first 4 bits

            char *im = strtok(NULL, " \r\n");//IMM
            char *R1 = strtok(NULL, " \r\n");//R1
                r1 = (short int) atoi(R1 + 1); // R1 -> 1
                r1 = r1<<6;

            immediate = (short int) atoi(im + 1); 
            parsed = opcode | r1 | immediate;

        }
        else if (strcmp(command, "SAR") == 0) {
            opcode = 9;
            opcode = opcode<<12;//put opcode in first 4 bits

            char *im = strtok(NULL, " \r\n");//IMM
            char *R1 = strtok(NULL, " \r\n");//R1
                r1 = (short int) atoi(R1 + 1); // R1 -> 1
                r1 = r1<<6;

            immediate = (short int) atoi(im + 1);
            parsed = opcode | r1 | immediate;

        }
        else if (strcmp(command, "LDR") == 0) {
            opcode = 10;
            opcode = opcode<<12;//put opcode in first 4 bits

            char *im = strtok(NULL, " \r\n");//IMM
            char *R1 = strtok(NULL, " \r\n");//R1
                r1 = (short int) atoi(R1 + 1); // R1 -> 1
                r1 = r1<<6;

            immediate = (short int) atoi(im + 1); 
            parsed = opcode | r1 | immediate;

        }
        else if (strcmp(command, "STR") == 0) {
            opcode = 11;
            opcode = opcode<<12;//put opcode in first 4 bits

            char *im = strtok(NULL, " \r\n");//IMM
            char *R1 = strtok(NULL, " \r\n");//R1
                r1 = (short int) atoi(R1 + 1); // R1 -> 1
                r1 = r1<<6;


            immediate = (short int) atoi(im + 1); 
            parsed = opcode | r1 | immediate;

        }
        else{
            printf("invalid syntax\n");
    
        }  
        //store in instruction memory
        instMem[instPtr++] = parsed;
        
    }
    

}

void decode(short int instruction){
    short int opcode = 0;//both I and R
    short int r1 = 0;//both I and R
    short int r2 = 0;//R
    short int immediate = 0;//I

    int8_t r1Value = 0;
    int8_t r2Value = 0;

}

void alu(int8_t opA, int8_t opB, short int opcode){
    int8_t output = 0;
    //will change value of SREG based on the operation
    switch(opcode){
        case 0: ; break;//add - affects C,V,N,S,Z
        case 1: ; break;//sub - affects V,N,S,Z
        case 2: ; break;//mul - affects N,Z
        case 3: ; break;//movi 
        case 4: ; break;//beqz
        case 5: ; break;//andi - affects N,Z
        case 6: ; break;//eor - affects N,Z
        case 7: ; break;//br
        case 8: ; break;//sal - affects N,Z
        case 9: ; break;//sar - affects N,Z
        case 10: ; break;//ldr
        case 11: ; break;//str
        default:
            printf("operation doesn't exist");
            break;
    }

}

void fetch(){
    short int instruction = 0;
        for(int i =0;i<numInst;i++){
            instruction = instMem[pc];
            decode(instruction);
            pc++;
        }
}

void execute(){
    
}

int main(){
    // char test[] = "R1";
    // char test2[] = "R12";

    // int num1 = atoi(test + 1);
    // int num2 = atoi(test2 +1);

    // printf("R%d",num1);
    // printf("R%d",num2);

    // short int op = 1<<12;//0001000000000000
    // short int r1 = 2<<6;//0000000010000000
    // short int r2 = 3;//0000000000000011
    // short int parsed = op | r1 | r2;//0001000010000011 - 4227
    // printf("%d",parsed);

    parseandStore("program.txt");
    printf("%d", instMem[0]);


    return 0;

}


