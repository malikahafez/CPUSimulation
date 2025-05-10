#include <stdint.h>

//clock cycle counter
int cycles = 0;

//instruction memory
short int instMem[1024];//16-bit inst
int numInst = sizeof(instMem)/sizeof(short int);
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
void parseAndStore(char* filename){

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

void main(){

}


