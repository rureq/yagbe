#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


uint8_t * readROM(char * filename, long int * size){
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
    uint8_t * rom = readROM("Pokemon.gb", &length);
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