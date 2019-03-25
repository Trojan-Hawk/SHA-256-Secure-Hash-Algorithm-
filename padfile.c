#include <stdio.h>
#include <stdint.h>

// union of memory space
union msgBlock {
    uint8_t e[64];
    uint32_t t[16];
    uint64_t s[8];
};

// enum status flag
enum status (READ, PAD0, PAD1, FINISH);


int main(int argc, char *argv[]) {

    union msgBlock M;
    
    uint64_t nobits = 0;
    // track the number of bytes being read in
    uint64_t nobytes;

    // set the status flag to READ
    enum status S = READ;

    FILE* file;
    file = fopen(argv[1], "r");

    // need to preform some error check on file opening
    // f_error/f_err maybe

    while(S == READ) {
        nobytes = fread(M.e, 1, 64, file);
        // keep track of the number of bits
        nobits = nobits + (nobytes * 8);
        // if less than 56 bytes are read, current message block can just be padded
        if(nobytes < 56) {
            printf("I've found a block with less than 55 bytes!\n");//DEBUG
            // append '10000000' to the message block
            M.e[nobytes] = 0x80;
            // pad all the bits until 64 bits left
            while(nobytes < 56) {
                nobytes = nobytes +1;
                // append on all '0' bits
                M.e[nobytes] = 0x00;
            }// while
            // append the number of bytes in the file as a 64 bit int
            M.s[7] = nobits;// IS THIS BIG ENDIAN???****************************************
            // set the flag to FINISH
            S = FINISH;
        }// if
        // if we have read more than 56 bytes but less than 64 bytes,
        // there is not enough space to append 1 + 0* + 64bit total
        // an extra message block of padding will be needed here
        else if (nobytes < 64) {
            // set the flag
            S = PAD0;
            // append the 1 to the message
            M.e[nobytes] = 0x80;
            // pad the rest of the message block with '0' bits
            while(nobytes < 64) {
                nobytes = nobytes +1;
                M.e[nobytes] = 0x00;
            }// while
        }// else if
        printf("%llu\n", nobytes);//DEBUG
    }// while
    
    fclose(file);
    
    // print check DEBUG
    for(int i = 0; i < 64; i++){
        printf("%x ", M.e[i]);
    }// for
    printf("\n");

    return 0;
}// main
