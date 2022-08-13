#include <stdio.h>
#define blockSize 2

int main(int argc, char *argv[]) {
    FILE *file;
    unsigned char block[blockSize];
    char calc_cc(int);
    int byte, count = 0;
    file = fopen(argv[1], "rb");
    int mem[100], registers[8] = {0}, pc = 0, ir = 0, offset = 0, reg = 0, cc = 0;
    do {
        byte = fread(block, 1, blockSize, file);
        if (byte != 2) break;
        mem[count] = (block[0] << 8) | block[1];
        count ++;
    } while(1);
    pc = mem[0];
    for (int i = 1; i < count; i++) {
        int line = mem[i];
        pc ++;
        ir = mem[i];
        //LD
        if ((line & 0xf000) == 0x2000){
            offset = line & 0x01ff;
            reg = (line & 0x0eff) >> 9;
            registers[reg] = mem[offset + i + 1];
            cc = registers[reg];
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
        else if ((line & 0xf000) == 0xe000){
            offset = line & 0x01ff;
            reg = (line & 0x0eff) >> 9;
            registers[reg] = pc + offset;
            cc = registers[reg];
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
        else if ((line & 0xf000) == 0xa000){
            offset = line & 0x01ff;
            reg = (line & 0x0eff) >> 9;
            int address1 = mem[offset + i + 1];
            int address2 = address1 - mem[0];
            registers[reg] = mem[address2 + 1];
            cc = address1 - line;
            printf("after executing instruction\t0x%04x\n", mem[i]);
            for(int i = 0; i < 8; i++) {
                printf("R%d\t0x%04x\n", i, registers[i]);
            }
            printf("PC\t0x%04x\n", pc);
            printf("IR\t0x%04x\n", ir);
            calc_cc(registers[reg]);
            printf("==================\n");
        }
        //AND
        else if ((line & 0xf000) == 0x5000){
            int dr = (line & 0x0eff) >> 9;
            int sr1 = (line & 0x01c0) >> 6;
            int mode = (line & 0x0020) >> 5;
            if (mode == 1) { //immediate
                int num = (line & 0x001f);
                registers[dr] = registers[sr1] & num;
            }
            else { //register
                int sr2 = (line & 0x0007);
                registers[dr] = registers[sr1] & registers[sr2];
            }
            cc = registers[dr];
            printf("after executing instruction\t0x%04x\n", mem[i]);
            for(int i = 0; i < 8; i++) {
                printf("R%d\t0x%04x\n", i, registers[i]);
            }
            printf("PC\t0x%04x\n", pc);
            printf("IR\t0x%04x\n", ir);
            calc_cc(registers[reg]);
            printf("==================\n");
        }
        //NOT
        else if ((line & 0xf000) == 0x9000){
            int dr = (line & 0x0eff) >> 9;
            int sr = (line & 0x01c0) >> 6;
            registers[dr] = ~registers[sr];
            cc = registers[dr];
            registers[dr] = registers[dr] & 0x0000ffff;
            printf("after executing instruction\t0x%04x\n", ir);
            for(int i = 0; i < 8; i++) {
                printf("R%d\t0x%04x\n", i, registers[i]);
            }
            printf("PC\t0x%04x\n", pc);
            printf("IR\t0x%04x\n", ir);
            calc_cc(cc);
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