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

struct registers{
    uint8_t A, B, C, D, E, F, H, L;
    uint16_t PC, SP; 
};

typedef struct registers Registers;

union Uregisters{
    struct regs{
        uint8_t A, F, B, C, D, E, H, L;
    } regs;
    struct dregs{
        uint16_t AF, BC, DE, HL;
    } dregs;
};



struct flags{
    bool Z, N, H, CY;
};

uint8_t readbyte(uint8_t *gamefile, uint32_t address){
    return gamefile[address];
};

void operate(uint8_t opcode, byte Loperand, char Roperand, Registers *regs){
    if (opcode == 0x01)
    {
        
    }
}

uint8_t *readROM(char * filename, long int * size){
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

int main(){
    long int length = 0;
    uint8_t *rom = readROM("Pokemon.gb", &length);
       printf("Pokemon ROM:\n");
//       for (int i = 0; i <length; i++){
//        printf("0x%x ",rom[i]);
//         if((i+1)%16 == 0)
//            printf("\n");
//       }
        int a = 0;
    char title[16];
        for(int i = 0; i < 16; i++){
            title[i] = rom[i+0x134];
            printf("%c",title[i]); 
        }
    while(1){
      scanf("%d",&a);
    }

    free(rom);
    return 0;
}