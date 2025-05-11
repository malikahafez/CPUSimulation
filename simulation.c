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
    short int rS = 0;//both I and R
    short int rT = 0;//R
    short int immAdr = 0;//I

    int8_t rSValue = 0;
    int8_t rTValue = 0;

    opcode = (0b1111000000000000 & instruction)>>12;
    rS = (0b0000111111000000 & instruction)>>6;
    rT = (0b0000000000111111 & instruction);
    immAdr = (0b0000000000111111 & instruction);

    rSValue = regFile[rS];//value in source register in register file
    rTValue = regFile[rT];//value in target register in register file

     printf("Instruction %i\n",pc);
		printf("opcode = %i\n",opcode);
		printf("rs = %i\n",rS);
		printf("rt = %i\n",rT);
		printf("immediate = %i\n",immAdr);
		printf("value[rs] = %i\n",rSValue);
		printf("value[rt] = %i\n",rTValue);
		printf("---------- \n");

}

void execute(short int rs, short int rt_imm_addr, short int opcode){
    //rt_imm_addr could also be immediate
    short int output = 0;
    //sreg flags
    //0b0000CVNSZ
    int8_t C = 0; 
    int8_t V = 0; 
    int8_t N = 0; 
    int8_t S = 0; 
    int8_t Z = 0; 
    //will change value of SREG based on the operation
    switch(opcode){
        case 0: //add - affects C,V,N,S,Z
            output = regFile[rs] + regFile[rt_imm_addr];
            regFile[rs] = (int8_t)output;//rs = rs + rt_imm_addr
            C =(int8_t)((output & 0b000000100000000)>>8);
            if((regFile[rs]>0 && regFile[rt_imm_addr]>0 && output<0) || (regFile[rs]<0 && regFile[rt_imm_addr]<0 && output>0) ){
                V = 1;//00000001
            }
            if(output<0){N = 1;}
            S = N ^ V;
            if(output == 0){Z = 1;}
        break;
        case 1: //sub - affects V,N,S,Z
            output = regFile[rs] - regFile[rt_imm_addr];
            regFile[rs] = (int8_t)output;//rs = rs - rt_imm_addr
            if((regFile[rs]>0 && regFile[rt_imm_addr]>0 && output<0) || (regFile[rs]<0 && regFile[rt_imm_addr]<0 && output>0) ){
                V = 1;//00000001
            }
            if(output<0){N = 1;}
            S = N ^ V;
            if(output == 0){Z = 1;}
        break;
        case 2: //mul - affects N,Z
            output = regFile[rs] * regFile[rt_imm_addr];
            regFile[rs] = (int8_t)output;//rs = rs * rt_imm_addr
            if(output<0){N = 1;}
            if(output == 0){Z = 1;}
        break;
        case 3: //movi 
            regFile[rs] = (int8_t)rt_imm_addr;
        break;
        case 4:  //beqz
            if(regFile[rs] == 0){
                pc = pc + rt_imm_addr;
            }
        break;
        case 5: //andi - affects N,Z
            output = regFile[rs] & (int8_t)rt_imm_addr;
            regFile[rs] = (int8_t)output;//rs = rs AND rt_imm_addr
            if(output < 0){N = 1;}
            if(output == 0){Z = 1;}
        break;
        case 6: //eor - affects N,Z
            output = regFile[rs] ^ regFile[rt_imm_addr];
            regFile[rs] = (int8_t)output;//rs = rs XOR rt_imm_addr
            if(output<0){N = 1;}
            if(output == 0){Z = 1;}
        break;
        case 7: //br
            short int r1val = ((short int) regFile[rs])<<6;//0bRRRRRRRR00000000
            short int r2val = (short int) regFile[rs];//0b00000000RRRRRRRR
            pc = r1val | r2val;//0bRRRRRRRRRRRRRRRR
        break;
        case 8:  //sal - affects N,Z
            output = regFile[rs] << rt_imm_addr;
            regFile[rs] = (int8_t)output;//rs = rs SAL rt_imm_addr
            if(output<0){N = 1;}
            if(output == 0){Z = 1;}

        break;
        case 9: //sar - affects N,Z
            output = regFile[rs] >> rt_imm_addr;
            regFile[rs] = (int8_t)output;//rs = rs SAR rt_imm_addr
            if(output<0){N = 1;}
            if(output == 0){Z = 1;}
        break;
        case 10: //ldr
            regFile[rs] = dataMem[rt_imm_addr];
        break;
        case 11: //str
            dataMem[rt_imm_addr] = regFile[rs];
        break;
        default:
            printf("operation doesn't exist");
            break;
    }
    //should sreg be updated with every inst execution or retain old values when not updated????
     C = C<<4;
     V = V<<3;
     N = N<<2;
     S = S<<1;
    SREG = C | V | N | S | Z;

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

    // parseandStore("program.txt");
    // printf("%d", instMem[0]);

    // decode(0b0001000010000011);

    short int output = 258;
    int8_t C =(int8_t)((output & 0b000000100000000)>>8);

    printf("%d", C);


    return 0;

}


