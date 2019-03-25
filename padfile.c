#include <stdio.h>
#include <stdint.h>

// union of memory space
union msgBlock {
    uint8_t e[64];
    uint32_t t[16];
    uint64_t s[8];
};

int main(int argc, char *argv[]) {

    union msgBlock M;

    uint64_t nobits = 0;

    uint64_t nobytes;

    FILE* file;
    file = fopen(argv[1], "r");

    // need to preform some error chech on file opening
    // f_error/f_err maybe

    while(!feof(file)) {
        nobytes = fread(M.e, 1, 64, file);
        // keep track of the number of bits
        nobits = nobits + (nobytes * 8);
        if(nobytes < 56) {
            printf("I've found a block with less than 55 bytes!\n");
            // add '00000001' to the message block
            M.e[nobytes] = 0x01;
            
            while(nobytes < 56) {
                nobytes = nobytes +1;
                // append on all '0' bits
                M.e[nobytes] = 0x00;
            }// while

            M.s[7] = nobits;
        }// if
        printf("%llu\n", nobytes);
    }// while
    
    fclose(file);
    
    // print check
    for(int i = 0; i < 64; i++){
        printf("%x ", M.e[i]);
    }// for
    printf("\n");

    return 0;
}// main
