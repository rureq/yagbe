#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define A_ID 0b111;
#define B_ID 0b000;
#define C_ID 0b001;
#define D_ID 0b101;
#define E_ID 0b011;
#define H_ID 0b100;
#define L_ID 0b101;

#define ROM_OFFSET = 0x0000;
#define VRAM_OFFSET = 0x8000;
#define ERAM_OFFSET = 0xA000;
#define WRAM_OFFSET = 0xC000;
#define SAT_OFFSET = 0xFE00; //sprite attribute table
#define IO_REGS_OFFSET = 0xFF00;
#define HRAM_OFFSET = 0xFF80;
#define IE_OFFSET = 0xFFFF; //Interrupt Enable register



union Registers{
    struct regs{
        bool Z, N, Hf, Cf;
        uint8_t A, F, B, C, D, E, H, L;
    } regs;
    struct dregs{
        bool Z, N, Hf, Cf;
        uint16_t FA, CB, ED, LH, SP, PC;
    } dregs;
};

struct flags{
    bool Z, N, H, CY;
};

uint8_t readbyte(uint8_t *gamefile, uint32_t address){
    return gamefile[address];
};

uint16_t read2bytes(uint8_t *gamefile, uint32_t address){
    uint16_t twobytes;
    twobytes = (gamefile[address+1] << 8) | gamefile[address]; //is endianness correct?
    return twobytes;
}

void writebyte(uint16_t address, uint8_t *memory, uint8_t data){
    memory[address] = data;
};
void write2bytes(uint16_t address, uint8_t *memory, uint16_t data){
    
};

uint8_t getop(uint8_t *gamefile, union Registers *regs){
    return gamefile[regs->dregs.PC];
}

union operation{
    struct op{
        char* name;
        uint8_t opcode;
        uint8_t length;
        uint8_t cycles;
        uint8_t flags : 4; 
    };
};

bool check4halfcarry(uint8_t a, uint8_t b){
    if ((((a&0xf) + (b&0xf)) &0x10) == 0x10){
        return true;
    }
    return false;
}

void perfop(uint8_t opcode, union Registers *regs, uint8_t *memory){
    switch (opcode)
    {
        case 0x00: //NOP, 1,4, ----
        {
            regs->dregs.PC++;
            break;
        }
        case 0x01: //LD BC, d16, 3, 12, ----
        {
            regs->dregs.CB = read2bytes(memory, memory[regs->dregs.PC+1]);
            regs->dregs.PC += 3;   
            break;
        }
        case 0x02: //LD (BC), A, 1,  8, ----
        {
            memory[read2bytes(memory, regs->dregs.CB)] = regs->regs.A;
            regs->dregs.PC++;  
            break;
        }
        case 0x03: //INC BC, 1,  8, ----
        {
            regs->dregs.CB++;
            regs->dregs.PC++;
            break;
        }
        case 0x04: //INC B, 1, 4, Z0H-
        {
            regs->regs.B++;
            regs->dregs.PC++;
            if (regs->regs.B == 0){
                regs->regs.F |= 0b10000000;
            }
            regs->regs.F ^= 0b01000000;
            if(check4halfcarry(regs->regs.B-1, 1)){
                regs->regs.F |= 0b00100000;
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return;
}
void readROMNEW(char *filename, uint8_t *memory, long int *len){
    
    FILE* romFile = fopen(filename, "rb");
    if (romFile != NULL){
        fseek(romFile, 0, SEEK_END);
        long int length = ftell(romFile);
        *len = length;
        rewind(romFile); 
        fread(memory,1,length,romFile);
        fclose(romFile);
    } 
    else{
        printf("Error reading ROM file\n");
    }

}

uint8_t *readROM(char *filename, long int *size){
    FILE* romFile = fopen(filename, "rb");
    uint8_t * pointer = NULL; 
    if (romFile != NULL){
        fseek(romFile, 0, SEEK_END);
        long int length = ftell(romFile);
        rewind(romFile); 
        pointer = malloc(sizeof(uint8_t)*length);   
        fread(pointer,1,length,romFile);
        fclose(romFile);
        *size = length;
    } 
    else{
        printf("Error reading ROM file\n");
    }
    
    return pointer; 
}

uint8_t *initRAM(){
    uint8_t *RAM = malloc(sizeof(uint8_t)*16000);
     if(!*RAM){
        printf("Error allocating memory\n");
        return NULL;
    }
    return RAM;
};
uint8_t *initmemory(){
    uint8_t *mem = malloc(sizeof(uint8_t)*0xFFFF);
    if(!*mem){
        printf("Error allocating memory\n");
        return NULL;
    }
    return mem;
};

int main(){
    union Registers registers;
    registers.regs.F = 0;
    uint8_t *mem = initmemory();
    long int length = 0;
    readROMNEW("snake.gb", mem, &length);
    
    
    //uint8_t *rom = readROM("Pokemon.gb", &length); // old reading function
    printf("Snake ROM:\n");

    char title[16];
    for(int i = 0; i < 16; i++){
        title[i] = mem[i+0x134];
        printf("%c",title[i]); 
    }
    printf("\n");
    printf("ROM size: %d\n",length);
    registers.dregs.PC = 0x101;
    
    uint8_t firstOP = getop(mem, &registers);
    printf("first op code: %x\n", firstOP);

    uint16_t secOP = read2bytes(mem, 0x102);
    printf("first read two bytes: %x\n", secOP);
    registers.regs.Hf = 0;
    printf("half carry: %d\n", registers.regs.Hf);
    registers.regs.B = 31;
    perfop(0x04, &registers, mem);
    printf("result: %d\n", registers.regs.B);
    printf("half carry: %d\n", (registers.regs.F&0b00100000)>>5); //extracts half carry bit from flag register



    int a = 0; //prevents program from dissapearing too quickly
    while(1){
      scanf("%d",&a);
    }

    free(mem);
    return 0;
}