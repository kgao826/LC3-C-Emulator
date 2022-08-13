#include <stdio.h>
#define blockSize 2

int main(int argc, char *argv[]) {
        FILE *file;
        unsigned char block[blockSize];
        int byte, i;
        file = fopen(argv[1], "rb");
        do {
                printf("0x");
                byte = fread(block, 1, blockSize, file);
                if (byte != 2) break;
                printf("%02x", block[0]);
                printf("%02x\n", block[1]);
        } while(1);
}

