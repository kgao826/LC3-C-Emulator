#include <stdio.h>
#define blockSize 2

int main(int argc, char *argv[]) {
    FILE *file;
    unsigned char block[blockSize];
    char calc_cc(int), cc[] ="Z";
    int byte, count = 0;
    file = fopen(argv[1], "rb");
    int mem[100], registers[8] = {0}, pc = 0, ir = 0, offset = 0, reg = 0;
    do {
        byte = fread(block, 1, blockSize, file);
        if (byte != 2) break;
        mem[count] = (block[0] << 8) | block[1];
        count ++;
    }while(1);
    pc = mem[0];
    for (int i = 1; i < count; i++) {
        int line = mem[i];
        pc ++;
        //LD
        if ((line & 0xf000) == 0x2000){
            offset = line & 0x01ff;
            reg = (line & 0x0eff) >> 9;
            registers[reg] = mem[offset + i + 1];
            ir = mem[i];
            printf("after executing instruction\t0x%04x\n", mem[i]);
            for(int i = 0; i < 8; i++) {
                printf("R%d\t0x%04x\n", i, registers[i]);
            }
            printf("PC\t0x%04x\n", pc);
            printf("IR\t0x%04x\n", ir);
            calc_cc(registers[reg]);
            printf("==================\n");
        }
        //LEA
        if ((line & 0xf000) == 0xe000){
            offset = line & 0x01ff;
            reg = (line & 0x0eff) >> 9;
            registers[reg] = pc + offset;
            ir = mem[i];
            printf("after executing instruction\t0x%04x\n", mem[i]);
            for(int i = 0; i < 8; i++) {
                printf("R%d\t0x%04x\n", i, registers[i]);
            }
            printf("PC\t0x%04x\n", pc);
            printf("IR\t0x%04x\n", ir);
            calc_cc(registers[reg]);
            printf("==================\n");
        }
        //LDI
        if ((line & 0xf000) == 0xa000){
            offset = line & 0x01ff;
            reg = (line & 0x0eff) >> 9;
            int address1 = mem[offset + i + 1];
            int address2 = address1 - mem[0];
            registers[reg] = mem[address2 + 1];
            ir = mem[i];
            printf("after executing instruction\t0x%04x\n", mem[i]);
            for(int i = 0; i < 8; i++) {
                printf("R%d\t0x%04x\n", i, registers[i]);
            }
            printf("PC\t0x%04x\n", pc);
            printf("IR\t0x%04x\n", ir);
            calc_cc(address1 - line);
            printf("==================\n");
        }
        //HALT
        else if (line  == 0xf025){
            break;
        }
    }
}

char calc_cc(int num) {
    if (num == 0){
        printf("CC\tZ\n");
    }
    else if (num > 0){
        printf("CC\tP\n");
    }
    else {
        printf("CC\tN\n");
    }
}