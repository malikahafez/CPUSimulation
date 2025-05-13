#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//clock cycle counter
int cycles = 0;
//struct for fetch-decode
typedef struct {
    short int instruction;
    bool valid;
} IF_ID_Stage;
//struct for decode-execute
typedef struct {
    short int instruction;
    bool valid;
} ID_EX_Stage;
//instruction memory
// Global or static variables
IF_ID_Stage if_id = {.valid = false};
ID_EX_Stage id_ex = {.valid = false};
void flushPipeline() {
    if_id.valid = false;
    id_ex.valid = false;
}//to be added in execute stage
short int instMem[1024] = {0};//16-bit inst
int numInst = sizeof(instMem)/sizeof(short int);
int instPtr = 0;
//0b0000000000000000

//data memory
int8_t dataMem[2048] = {0};//8-bit data
int numData = sizeof(dataMem)/sizeof(int8_t);
//0b00000000

//register file (64 GPRs)
int8_t regFile[64] = {0};//8-bit registers
int numReg = sizeof(regFile)/sizeof(int8_t);

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
        cycles++;
        
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
		printf("------------------------------ \n");

}

// void execute(short int rs, short int rt_imm_addr, short int opcode){
//     //rt_imm_addr could also be immediate
//     short int output = 0;
//     short int r1val;
//     short int r2val;
//     //sreg flags
//     //0b0000CVNSZ
//     uint8_t C = 0; 
//     uint8_t V = 0; 
//     uint8_t N = 0; 
//     uint8_t S = 0; 
//     uint8_t Z = 0; 

//     int8_t a  = 0;
//     int8_t b = 0;
//     int8_t res = 0;
//     //will change value of SREG based on the operation
//     switch(opcode){
//         case 0: //add - affects C,V,N,S,Z
//             output = regFile[rs] + regFile[rt_imm_addr];
//             regFile[rs] = (int8_t)output;//rs = rs + rt_imm_addr
//             uint16_t result = (uint8_t)regFile[rs] + (uint8_t)regFile[rt_imm_addr];
//             C = (result > 0xFF) ? 1 : 0;
//             // if((regFile[rs]>0 && regFile[rt_imm_addr]>0 && (int8_t)output<0) || (regFile[rs]<0 && regFile[rt_imm_addr]<0 && (int8_t)output>0) ){
//             //     V = 1;//00000001
//             // }
//             //overflow = Sum[8] XOR X[7] XOR Y[7] XOR Sum[7]
//             uint8_t sum8 = (uint8_t)(((output & 0b000000100000000)>>8)&1);
//             uint8_t x7 = ((0b10000000 & (uint8_t)regFile[rs])>>7)&1;
//             uint8_t y7 = ((0b10000000 & (uint8_t)regFile[rt_imm_addr])>>7)&1;
//             uint8_t sum7 = (uint8_t)(((output & 0b000000010000000)>>7)&1);
//             V = sum8 ^ x7 ^ y7 ^ sum7;
//             // a = regFile[rs];
//             // b = regFile[rt_imm_addr];
//             // res = (int8_t)result;
//             // V = ((a > 0 && b > 0 && res < 0) || (a < 0 && b < 0 && res > 0)) ? 1 : 0;
//             if((int8_t)output<0){N = 1;}
//             S = N ^ V;
//             if((int8_t)output == 0){Z = 1;}
//         break;
//         case 1: //sub - affects V,N,S,Z
//             output = regFile[rs] - regFile[rt_imm_addr];
//             regFile[rs] = (int8_t)output;//rs = rs - rt_imm_addr
//             // if((regFile[rs]>0 && regFile[rt_imm_addr]>0 && (int8_t)output<0) || (regFile[rs]<0 && regFile[rt_imm_addr]<0 && (int8_t)output>0) ){
//             //     V = 1;//00000001
//             // }
//             a = regFile[rs];
//             b = regFile[rt_imm_addr];
//             res = a - b;
//             V = ((a > 0 && b < 0 && res < 0) || (a < 0 && b > 0 && res > 0)) ? 1 : 0;
//             if((int8_t)output<0){N = 1;}
//             S = N ^ V;
//             if((int8_t)output == 0){Z = 1;}
//         break;
//         case 2: //mul - affects N,Z
//             output = regFile[rs] * regFile[rt_imm_addr];
//             regFile[rs] = (int8_t)output;//rs = rs * rt_imm_addr
//             if((int8_t)output<0){N = 1;}
//             if((int8_t)output == 0){Z = 1;}
//         break;
//         case 3: //movi 
//             regFile[rs] = (int8_t)rt_imm_addr;
//         break;
//         case 4:  //beqz
//             if(regFile[rs] == 0){
//                 pc = pc + rt_imm_addr;
//             }
//         break;
//         case 5: //andi - affects N,Z
//             output = regFile[rs] & (int8_t)rt_imm_addr;
//             regFile[rs] = (int8_t)output;//rs = rs AND rt_imm_addr
//             if((int8_t)output < 0){N = 1;}
//             if((int8_t)output == 0){Z = 1;}
//         break;
//         case 6: //eor - affects N,Z
//             output = regFile[rs] ^ regFile[rt_imm_addr];
//             regFile[rs] = (int8_t)output;//rs = rs XOR rt_imm_addr
//             if((int8_t)output<0){N = 1;}
//             if((int8_t)output == 0){Z = 1;}
//         break;
//         case 7: //br
//             r1val = ((short int) regFile[rs])<<6;//0bRRRRRRRR00000000
//             r2val = (short int) regFile[rt_imm_addr];//0b00000000RRRRRRRR
//             pc = r1val | r2val;//0bRRRRRRRRRRRRRRRR
//         break;
//         case 8:  //sal - affects N,Z
//             output = regFile[rs] << rt_imm_addr;
//             regFile[rs] = (int8_t)output;//rs = rs SAL rt_imm_addr
//             if((int8_t)output<0){N = 1;}
//             if((int8_t)output == 0){Z = 1;}

//         break;
//         case 9: //sar - affects N,Z
//             output = regFile[rs] >> rt_imm_addr;
//             regFile[rs] = (int8_t)output;//rs = rs SAR rt_imm_addr
//             if((int8_t)output<0){N = 1;}
//             if((int8_t)output == 0){Z = 1;}
//         break;
//         case 10: //ldr
//             regFile[rs] = dataMem[rt_imm_addr];
//         break;
//         case 11: //str
//             dataMem[rt_imm_addr] = regFile[rs];
//         break;
//         default:
//             printf("operation doesn't exist");
//             break;
//     }
//     //should sreg be updated with every inst execution or retain old values when not updated????
//      C = C<<4;
//      printf("%d\n", C);
//      V = V<<3;
//      printf("%d\n", V);
//      N = N<<2;
//      printf("%d\n", N);
//      S = S<<1;
//      printf("%d\n", S);
//      printf("%d\n",Z);
//     SREG = C | V | N | S | Z;
//     for (int i = 7; i >= 0; i--) {
//         printf("%d", (SREG >> i) & 1);
//     }

// }
//FIXED 
void execute(short int rs, short int rt_imm_addr, short int opcode) {
    short int output = 0;
    // int8_t r1val, r2val;
    short int r1val;
    short int r2val;
    
    //sreg flags
    //0b0000CVNSZ
        uint8_t C = 0; 
        uint8_t V = 0; 
        uint8_t N = 0; 
        uint8_t S = 0; 
        uint8_t Z = 0; 
    //will change value of SREG based on the operation

    bool sreg_updated = false;

    switch(opcode) {
        case 0: // ADD - affects C,V,N,S,Z
        {
            printf("ADD instruction between R%d = %d and R%d = %d:\n", rs, rt_imm_addr,regFile[rs],regFile[rt_imm_addr]);
            uint16_t result = (uint8_t)regFile[rs] + (uint8_t)regFile[rt_imm_addr];
            int8_t a = regFile[rs];
            int8_t b = regFile[rt_imm_addr];
            int8_t res = a + b;

            regFile[rs] = res;
            printf("        R%d value changed to %d\n",rs,res);
            C = (result > 0xFF);
            V = ((a > 0 && b > 0 && res <= 0) || (a < 0 && b < 0 && res >= 0));
            N = (res < 0);
            Z = (res == 0);
            S = N ^ V;
            sreg_updated = true;
        }
        break;

        case 1: // SUB - affects V,N,S,Z
        {
            printf("SUB instruction between R%d = %d and R%d = %d:\n", rs, rt_imm_addr,regFile[rs],regFile[rt_imm_addr]);
            int8_t a = regFile[rs];
            int8_t b = regFile[rt_imm_addr];
            int8_t res = a - b;

            regFile[rs] = res;
            printf("        R%d value changed to %d\n",rs,res);
            V = ((a > 0 && b > 0 && res <= 0) || (a < 0 && b < 0 && res >= 0));
            N = (res < 0);
            Z = (res == 0);
            S = N ^ V;
            sreg_updated = true;

        }
        break;

        case 2: // MUL - affects N,Z
        {
            printf("MUL instruction between R%d = %d and R%d = %d:\n", rs, rt_imm_addr,regFile[rs],regFile[rt_imm_addr]);
            int16_t result = regFile[rs] * regFile[rt_imm_addr];
            int8_t res = (int8_t)result;
            regFile[rs] = res;
            printf("        R%d value changed to %d\n",rs,res);
            N = (res < 0);
            Z = (res == 0);
            sreg_updated = true;

        }
        break;

        case 3: // MOVI - does NOT affect flags
            printf("MOVI instruction between R%d = %d and immediate = %d:\n", rs, rt_imm_addr,regFile[rs]);
            regFile[rs] = (int8_t)rt_imm_addr;
            printf("        R%d value changed to %d\n",rs,(int8_t)rt_imm_addr);
        break;

        case 4: // BEQZ - does NOT affect flags
            printf("BEQZ instruction using R%d = %d and immediate = %d:\n", rs, rt_imm_addr, regFile[rs]);
            if (regFile[rs] == 0) {
                pc += rt_imm_addr;
                printf("        Branch occured: PC value changed to %d\n",pc);
            }
            else{
                printf("        Branch did NOT occur: PC value did NOT change\n");
            }
            

        break;

        case 5: // ANDI - affects N,Z
        {
            printf("ANDI instruction between R%d = %d and immediate = %d:\n", rs, rt_imm_addr, regFile[rs]);
            int8_t res = regFile[rs] & (int8_t)rt_imm_addr;
            regFile[rs] = res;
            printf("        R%d value changed to %d\n",rs,res);
            N = (res < 0);
            Z = (res == 0);
            sreg_updated = true;

        }
        break;

        case 6: // EOR - affects N,Z
        {
            printf("EOR instruction between R%d = %d and R%d = %d:\n", rs, rt_imm_addr,regFile[rs],regFile[rt_imm_addr]);
            int8_t res = regFile[rs] ^ regFile[rt_imm_addr];
            regFile[rs] = res;
            printf("        R%d value changed to %d\n",rs,res);
            N = (res < 0);
            Z = (res == 0);
            sreg_updated = true;

        }
        break;

        case 7: // BR - does NOT affect flags
            printf("BR instruction using R%d = %d and R%d = %d:\n", rs, rt_imm_addr,regFile[rs],regFile[rt_imm_addr]);
            r1val = (0b0000000000000000 | regFile[rs])<<8;//0bRRRRRRRR00000000
            r2val = 0b0000000000000000 | regFile[rt_imm_addr];//0b00000000RRRRRRRR
            pc = r1val | r2val;//0bRRRRRRRRRRRRRRRR
            printf("        PC value changed to %d\n",pc);
            // r1val = regFile[rs] << 6;
            // r2val = regFile[rt_imm_addr];
            // pc = r1val | r2val;
        break;

        case 8: // SAL - affects N,Z
        {
            printf("SAL instruction between R%d = %d and immediate = %d:\n", rs, rt_imm_addr, regFile[rs]);
            int8_t res = regFile[rs] << rt_imm_addr;
            regFile[rs] = res;
            printf("        R%d value changed to %d\n",rs,res);
            N = (res < 0);
            Z = (res == 0);
            sreg_updated = true;

        }
        break;

        case 9: // SAR - affects N,Z
        {
            printf("SAR instruction between R%d = %d and immediate = %d:\n", rs, rt_imm_addr, regFile[rs]);
            int8_t res = regFile[rs] >> rt_imm_addr;
            regFile[rs] = res;
            printf("        R%d value changed to %d\n",rs,res);
            N = (res < 0);
            Z = (res == 0);
            sreg_updated = true;

        }
        break;

        case 10: // LDR - does NOT affect flags
            printf("LDR instruction using R%d = %d and address = %d:\n", rs, rt_imm_addr, regFile[rs]);
            regFile[rs] = dataMem[rt_imm_addr];
            printf("        R%d value changed to %d\n",rs, dataMem[rt_imm_addr]);

        break;

        case 11: // STR - does NOT affect flags
            printf("STR instruction using R%d = %d and address = %d:\n", rs, rt_imm_addr, regFile[rs]);
            dataMem[rt_imm_addr] = regFile[rs];
            printf("        Value in position %d in data memory changed to %d\n",rt_imm_addr, regFile[rs]);
        break;

        default:
            printf("Operation does NOT exist.\n");
            break;
    }

    // Update SREG 
        if(sreg_updated == true){
            printf("SREG Value changed:\n");
            printf("    Carry(C) Flag:%d\n", C);
            C = C<<4;
            printf("    Overflow(V) Flag:%d \n", V);
            V = V<<3;
            printf("    Negative(N) Flag:%d\n", N);
            N = N<<2;
           printf("    Sign(S) Flag:%d\n", S);
            S = S<<1;
            printf("    Zero(Z) Flag:%d\n",Z);
            SREG = C | V | N | S | Z;
            printf("SREG |0|0|0|C|V|N|S|Z|: ");
            for (int i = 7; i >= 0; i--) {
                printf("%d", (SREG >> i) & 1);
            }
            printf("\n");
        }
        else{
            printf("SREG Value did NOT change\n");
        }
        

}


void fetch(){
    short int instruction = 0;
        for(int i =0;i<cycles;i++){
            instruction = instMem[pc];

            decode(instruction);
            pc++;
             
        }
       
}



void pipeline() {
    // Execute stage
    if (id_ex.valid) {
        printf("Executing instruction: %d \n", id_ex.instruction);
        short int opcode = (0b1111000000000000 & id_ex.instruction)>>12;
        short int rs = (0b0000111111000000 & id_ex.instruction)>>6;
        short int rt_imm_addr = (0b0000000000111111 & id_ex.instruction);
        execute(rs, rt_imm_addr, opcode);
        printf("------------------------------\n");
        id_ex.valid = false; // Instruction has been executed
    }

    // Decode stage
    if (if_id.valid) {
    
        id_ex.instruction = if_id.instruction;
           printf("Decoding instruction: %d \n", id_ex.instruction);
        id_ex.valid = true;
        decode(id_ex.instruction);
        if_id.valid = false;
    }

    // Fetch stage
    if (pc < (3+(cycles-1)*1)) {
        printf("Fetching instruction at PC: %d \n", pc);
        printf("------------------------------\n");
        if_id.instruction = instMem[pc++];
        if_id.valid = true;
    }
}

void printRegisters(){
    printf("+---------------------------------------------------------------+\n");
    printf("|           Content of all Registers after Simulation           |\n");
    printf("+---------------------------------------------------------------+\n");
    printf("|            Register             |            Value            |\n");
    printf("+---------------------------------------------------------------+\n");
    for(int i = 0; i<numReg; i++){
        if(i<10){
            if(regFile[i]<10){
                printf("|               R%d                |              %d              |\n",i, regFile[i]);
                printf("|---------------------------------------------------------------|\n");

            }
            else{
                printf("|               R%d                |             %d              |\n",i,regFile[i]);
                printf("|---------------------------------------------------------------|\n");
            }
            
        }
        else{
            if(regFile[i]<10){
                printf("|               R%d               |              %d              |\n",i, regFile[i]);
                printf("|---------------------------------------------------------------|\n");

            }
            else{
                 printf("|               R%d               |             %d              | \n",i, regFile[i]);
                    printf("|---------------------------------------------------------------|\n");
            }
           
        }

    }


}

void printInstMem(){
printf("+---------------------------------------------------------------+\n");
    printf("|      Content of all Instruction Memory after Simulation       |\n");
    printf("+---------------------------------------------------------------+\n");
    printf("|            Position             |            Value            |\n");
    printf("+---------------------------------------------------------------+\n");
    for(int i = 0; i<numInst; i++){
        if(i<1000){
            if(instMem[i]<10){
                printf("|               MEM[%d]            |              %d              |\n",i, instMem[i]);
                printf("|---------------------------------------------------------------|\n");

            }
            else{
                printf("|               MEM[%d]            |             %d              |\n",i, instMem[i]);
                printf("|---------------------------------------------------------------|\n");

            }
            

        }
        else{
            if(instMem[i]<10){
                printf("|               MEM[%d]           |              %d              |\n",i, instMem[i]);
                printf("|---------------------------------------------------------------|\n");
            }
            else{
                printf("|               MEM[%d]           |             %d              |\n",i, instMem[i]);
                printf("|---------------------------------------------------------------|\n");

            }
            

        }
        
    }


}

void printDataMem(){
printf("+---------------------------------------------------------------+\n");
    printf("|          Content of all Data Memory after Simulation          |\n");
    printf("+---------------------------------------------------------------+\n");
    printf("|            Position             |            Value            |\n");
    printf("+---------------------------------------------------------------+\n");
    for(int i = 0; i<numData; i++){
        if(i<1000){
            if(dataMem[i]<10){
                printf("|               MEM[%d]            |              %d              |\n",i, dataMem[i]);
                printf("|---------------------------------------------------------------|\n");

            }
            else{
                printf("|               MEM[%d]            |             %d              |\n",i, dataMem[i]);
                printf("|---------------------------------------------------------------|\n");

            }
            

        }
        else{
            if(dataMem[i]<10){
                printf("|               MEM[%d]           |              %d              |\n",i, dataMem[i]);
                printf("|---------------------------------------------------------------|\n");
            }
            else{
                printf("|               MEM[%d]           |             %d              |\n",i, dataMem[i]);
                printf("|---------------------------------------------------------------|\n");

            }
            

        }
        
    }


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

     while (pc < (3+(cycles-1)*1)) {
        printf("\n      --- Cycle %d ---\n", pc + 1);
        pipeline();
    }

    //print all register values after last clock cycle:
    // printRegisters();
    
    //print full content of inst memory after last clock cycle:
    // printInstMem();

    //print full content of data memory after last clock cycle:
    // printDataMem();

   
    // decode(0b0001000010000011);

    // short int output = 258;
    // int8_t C =(int8_t)((output & 0b000000100000000)>>8);

    // printf("%d", C);

    // regFile[0] = -128;
    // regFile[1] = -128;
    // execute(0, 1, 0);//0b00011111
    // printf("%d\n",(uint8_t)((output & 0b000000100000000)>>8)&1);
    // uint8_t carry = (output >> 8) & 1;

    // printf("===== ADD Test (Positive Overflow) =====\n");
    // regFile[1] = 127; // Max positive value for int8_t
    // regFile[2] = 1;
    // execute(1, 2, 0); // ADD r1 = r1 + r2
    // printf("Result in r1: %d\n", regFile[1]);
    // printf("Expected: -128, V=1, C=0, N=1, S=0, Z=0\n\n");

    // printf("===== ADD Test (Negative Overflow and Carry) =====\n");
    // regFile[22] = -128; // Max positive value for int8_t
    // regFile[23] = -128;
    // execute(22, 23, 0); // ADD r22 = r22 + r23
    // printf("Result in r22: %d\n", regFile[22]);
    // printf("Expected: 0, V=1, C=1, N=0, S=1, Z=1\n\n");

    // printf("===== ADD Test (Carry with no Overflow) =====\n");
    // regFile[24] = 64; // Max positive value for int8_t
    // regFile[25] = -64;
    // execute(24, 25, 0); // ADD r1 = r1 + r2
    // printf("Result in r24: %d\n", regFile[24]);
    // printf("Expected: 0, V=0, C=1, N=0, S=0, Z=1\n\n");

    // printf("===== ADD Test (no Carry with Overflow) =====\n");
    // regFile[25] = 64; // Max positive value for int8_t
    // regFile[26] = 64; 
    // execute(25, 26, 0); // ADD r1 = r1 + r2
    // printf("Result in r25: %d\n", regFile[25]);
    // printf("Expected: 0, V=1, C=0, N=1, S=0, Z=0\n\n");

    // printf("===== SUB Test (Negative Result) =====\n");
    // regFile[3] = -50;
    // regFile[4] = 30;
    // execute(3, 4, 1); // SUB r3 = r3 - r4
    // printf("Result in r3: %d\n", regFile[3]);
    // printf("Expected: -80, V=0, N=1, S=0, Z=0\n\n");

    // printf("===== SUB Test ( Overflow ) =====\n");
    // regFile[28] = -128;
    // regFile[29] = 128;
    // execute(28, 29, 1); // SUB r28 = r28 - r29
    // printf("Result in r28: %d\n", regFile[28]);
    // printf("Expected: 0, V=1, N=0, S=1, Z=0\n\n");

    // printf("===== ANDI Test =====\n");
    // regFile[5] = 0b10101010;
    // execute(5, 0b00001111, 5); // ANDI r5 = r5 & 0b00001111
    // printf("Result in r5: %d\n", regFile[5]);
    // printf("Expected: 0b00001010, N=0, Z=0\n\n");

    // printf("===== EOR Test =====\n");
    // regFile[6] = 0b11110000;
    // regFile[7] = 0b00001111;
    // execute(6, 7, 6); // EOR r6 = r6 ^ r7
    // printf("Result in r6: %d\n", regFile[6]);
    // printf("Expected: 0b11111111 (-1), N=1, Z=0\n\n");

    // printf("===== MOVI Test =====\n");
    // execute(10, 123, 3); // MOVI r10 = 123
    // printf("Result in r10: %d\n", regFile[10]);
    // printf("Expected: 123\n\n");

    // printf("===== SAL Test =====\n");
    // regFile[11] = 0b00000001;
    // execute(11, 2, 8); // SAL r11 = r11 << 2
    // printf("Result in r11: %d\n", regFile[11]);
    // printf("Expected: 4, N=0, Z=0\n\n");

    // printf("===== SAR Test =====\n");
    // regFile[12] = -16; // 0b11110000
    // execute(12, 2, 9); // SAR r12 = r12 >> 2
    // printf("Result in r12: %d\n", regFile[12]);
    // printf("Expected: -4, N=1, Z=0\n\n");

    // printf("===== BEQZ Test =====\n");
    // regFile[13] = 0;
    // pc = 100;
    // execute(13, 10, 4); // if r13 == 0, pc += 10
    // printf("New PC: %d\n", pc);
    // printf("Expected: 110\n\n");

    // printf("===== LDR/STR Test =====\n");
    // regFile[20] = 42;
    // execute(20, 50, 11); // STR r20 -> mem[50]
    // printf("Data in mem[50]: %d\n", dataMem[50]);
    // regFile[21] = 0;
    // execute(21, 50, 10); // LDR mem[50] -> r21
    // printf("Data in r21: %d\n", regFile[21]);
    // printf("Expected: 42\n");

    // printf("===== BR Test =====\n");
    // regFile[50] = 1;
    // regFile[51] = 3;
    // // pc = 100;
    // execute(50, 51, 7); // pc = r50 || r51 = 00000001 || 00000011
    // printf("New PC: %d\n", pc);
    // for (int i = 15; i >= 0; i--) {
    //         printf("%d", (pc >> i) & 1);
    //     }
    // printf("\nExpected: 259\n\n");
    // printf("===== MUL Test (Negative Result) =====\n");
    // regFile[30] = 2; 
    // regFile[31] = -3; 
    // execute(30, 31, 2); // MUL r30 = r30 + r31
    // printf("Result in r30: %d\n", regFile[30]);
    // printf("Expected: 0, N=1, Z=0\n\n");
    // printf("===== MUL Test (Positive Result) =====\n");
    // regFile[32] = 2; 
    // regFile[33] = 3; 
    // execute(32, 33, 2); // MUL r32 = r32 + r33
    // printf("Result in r32: %d\n", regFile[32]);
    // printf("Expected: 0, N=0, Z=0\n\n");
    // printf("===== MUL Test (Zero Result) =====\n");
    // regFile[34] = 2; 
    // regFile[35] = 0; 
    // execute(34, 35, 2); // MUL r34 = r34 + r35
    // printf("Result in r34: %d\n", regFile[34]);
    // printf("Expected: 0, N=0, Z=1\n\n");

    return 0;

}


