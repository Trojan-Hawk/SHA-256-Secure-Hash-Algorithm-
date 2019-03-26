#include <stdio.h>
#include <stdint.h>

// union of memory space
union msgBlock {
    uint8_t e[64];
    uint32_t t[16];
    uint64_t s[8];
};

// enum status flag
enum status {READ, PAD0, PAD1, FINISH};

// macro that converts from little endian to big endian
// source: http://www.firmcodes.com/write-c-program-convert-little-endian-big-endian-integer/
# define lilEndianToBig(x) (((x>>24) & 0x000000ff) | ((x>>8) & 0x0000ff00) | ((x<<8) & 0x00ff0000) | ((x<<24) & 0xff000000))

int main(int argc, char *argv[]) {

    union msgBlock M;
    int i;
    uint64_t nobits = 0;
    // track the number of bytes being read in
    uint64_t nobytes;

    // set the status flag to READ
    enum status S = READ;
    // file pointer
    FILE* file;
    file = fopen(argv[1], "r");
    
    if (file == NULL) {
        // print the error
        perror(argv[1]);
        // return -1 for failure
        return(-1);
    } else {
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
                // convert to Big Endian
                M.s[7] = lilEndianToBig(nobits);
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
            // if the file byte size is a multiple of 64(512bits)
            // we need to create a new message block containing,
            // a '1' followed by 447 '0' bits followed by the 64bit total
            else if(feof(file)) {
                // set the flag
                S = PAD1;
            }// else if

            // if PAD0 or PAD1 flag is set, append '0' bits then 64 bit total
            if(S == PAD0 || S == PAD1) {
                for(i = 0; i < 56; i++)
                    M.e[i] = 0x00;
                // convert to Big Endian
                M.s[7] = lilEndianToBig(nobits);
            }// if
        
            // if the PAD1 flag is set, set the 0th bit to '1' 
            if(S == PAD1)
                M.e[0] = 0x80;
        
            printf("%llu\n", nobytes);//DEBUG
        
            //printf("Little Endian: %x", M.s[7]);
            //printf("Big Endian:    %x", lilEndianToBig(M.s[7]));
    
        }// while
    
        fclose(file);
    
        // print check DEBUG
        for(i = 0; i < 64; i++){
            printf("%x ", M.e[i]);
        }// for
        printf("\n");

        return 0;
    }// if/else
}// main
