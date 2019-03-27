// Timothy Cassidy
// G00333333
// link to SHA standard: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf

// The usual input/output header file
#include <stdio.h>
// For using fixed bit length integers
#include <stdint.h>
// string comparision
#include <string.h>

// enum status flag
enum status {READ, PAD0, PAD1, FINISH};

// macro functions
// see section 3.2 for definitions
#define ROTR(n,x)  ((x >> n) | (x << (32 - n)))      
// see section 4.1.2 for definitions
#define sig0(x)    (ROTR(7, x) ^ ROTR(18, x) ^ SHR(3, x))
#define sig1(x)    (ROTR(17, x) ^ ROTR(19, x) ^ SHR(10, x))
#define SHR(n,x)   (x >> n)
#define Ch(x,y,z)  ((x & y) ^ ((!x) & z)) // choosing  
#define Maj(x,y,z) ((x & y) ^ (x & z) ^ (y & z)) // majority vote
#define SIG0(x)    (ROTR(2, x) ^ ROTR(13, x) ^ ROTR(22, x))
#define SIG1(x)    (ROTR(6, x) ^ ROTR(11, x) ^ ROTR(25, x))
// macro that converts from little endian to big endian
// adapted from: http://www.firmcodes.com/write-c-program-convert-little-endian-big-endian-integer/
# define lilEndianToBig(x) (((x>>24) & 0x000000ff) | ((x>>8) & 0x0000ff00) | ((x<<8) & 0x00ff0000) | ((x<<24) & 0xff000000))

// union memory space message block
union msgBlock {
    uint8_t  e[64];
    uint32_t t[16];
    uint64_t s[8];
};

// struct to store program state
struct buffer_state {
    uint8_t verbose;
    uint8_t endianness;
    uint32_t M[16];
    uint64_t nobits;
    enum status S;
    FILE* file;
    uint64_t filePointer;
};

// function definitions
void sha256(struct buffer_state *);
int nextMsgBlock(struct buffer_state *);
int endianness();
void initializeState(struct buffer_state *, int, char*[]);

int main(int argc, char *argv[]) {
    // if no arguments passed, print help option
    if(argc <= 0){
        printf("Please input a valid file.");    
        return -1;
    }// if
        
    // declare the state struct
    struct buffer_state state;
    
    // initialize the state struct
    initializeState(&state, argc, argv); 

    // if verbose option is given
    if(state.verbose)
        state.file = fopen(argv[2], "r");
    else
        state.file = fopen(argv[1], "r");

    if (state.file == NULL) {
        // print the error
        if(state.verbose)
            perror(argv[2]);
        else
            perror(argv[1]);
        // return -1 for failure
        return(-1);
    } else {
        // pass the struct pointer to the sha256 algorithm
        sha256(&state);
        // close the file
        fclose(state.file);
        return 0;
    }// if/else

}// main

// sha256 algorithm implementation
void sha256(struct buffer_state* state) {
    // message schedule (section 6.2)
    uint32_t W[64];
    // working variables (section 6.2))
    uint32_t a, b, c, d, e, f, g, h;
    // temp variables (section 6.2)
    uint32_t T1, T2;
    
    // the hash value (section 6.2)
    // the values come from section 5.3.3
    uint32_t H[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    // K constants, see section 4.2.2 for definitions
    uint32_t K[] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
        0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 
        0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
        0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 
        0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 
        0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
        /*
          0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5
        , 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5
        , 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3
        , 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174
        , 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc
        , 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da
        , 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7
        , 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967
        , 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13
        , 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85
        , 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3
        , 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070
        , 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5
        , 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3
        , 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208
        , 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
        */
    };

    // for looping
    int t;
    // counter
    int i = 0;
    
    // get the first message block
    nextMsgBlock(state);

    // loop through the message blocks, see section 6.2.2 for definitions
    do {
        // increment the counter
        i++;
        // from page 22, W[t] = M[t] for 0 <= t <= 15
        for(t = 0; t < 16; t++){ 
            W[t] = state->M[t];
            printf("\nMessage section %d Value: %16x", t, W[t]);//DEBUGGING***************************************************        
        }// for

        printf("\n");//DEBUGGING***************************************************
        
        // from page 22, W[t] = ...
        for (t = 16; t < 64; t++){
            W[t] = sig1(W[t-2]) + W[t-7] + sig0(W[t-15]) + W[t-16];
        }// for

        // initilize a...h as per step 2, section 6.2.2
        a = H[0]; b = H[1]; c = H[2]; d = H[3];
        e = H[4]; f = H[5]; g = H[6]; h = H[7];
    
        // see section 3.2 for definitions
        for (t = 0; t < 64; t++){
            T1 = h + SIG1(e) + Ch(e, f, g) + K[t] + W[t];
            T2 = SIG0(a) + Maj(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }// for
        
        // step 4
        H[0] = a + H[0];
        H[1] = b + H[1];
        H[2] = c + H[2];
        H[3] = d + H[3];
        H[4] = e + H[4];
        H[5] = f + H[5];
        H[6] = g + H[6];
        H[7] = h + H[7];
        
        if(state->verbose)
            printf("PASS %d: %x %x %x %x %x %x %x %x\n", i, H[0], H[1], H[2], H[3], H[4], H[5], H[6], H[7]);
    } while(nextMsgBlock(state) == 0);     

}// sha25

// function to retrieve the next message block
int nextMsgBlock(struct buffer_state * state) {
    // if there is no data to read
    if(state->S == FINISH)
        return 1;
    
    union msgBlock M;
    int i;
    // track the number of bytes being read in
    uint64_t nobytes;
    FILE* file = state->file;

    // set the status flag to READ
    state->S = READ;

    // move the file pointer
    // nobits * 8, because there are 8 bits in a byte
    fseek(file, state->filePointer, SEEK_SET);
    nobytes = fread(M.e, 1, 64, file);
    
    // DEBUGGING
    nobytes = 3;
    M.e[0] = 0x61;
    M.e[1] = 0x62;
    M.e[2] = 0x63;
    M.e[3] = 0x00;

    for(i = 0; i < nobytes; i++) {
        printf("\nHex Value %d: %x",i,M.e[i]);
    }// for
    /*
    for(i = 0; i < 64; i++) {
       // M.e[i] = lilEndianToBig(M.e[i]);
    }
    
    for(i = 0; i < nobytes; i++) {
        printf("\nHex Value %d: %x",i,M.e[i]);
    }// for
    */
    // keep track of the number of bits
    state->nobits = state->nobits + (nobytes * 8);
    // keep track of the number of bytes
    state->filePointer = state->filePointer + nobytes;
    
    if(state->verbose) {
        printf("\nBytes Read: \t\t\t\t%d\n", nobytes);//DEBUG
        printf("Current file pointer:   \t\t%d\n", state->filePointer);
        printf("Number of bits: \t\t\t%d\n", state->nobits);
    }// if

    // if less than 56 bytes are read, current message block can just be padded
    if(nobytes < 56) {
        if(state->verbose)   
            printf("\nBlock with less than 55 bytes found.\n");//DEBUG
        // append '10000000' to the message block
        M.e[nobytes] = 0x80;
        // pad all the bits until 64 bits left
        while(nobytes < 56) {
            nobytes = nobytes +1;
            // append on all '0' bits
            M.e[nobytes] = 0x00;
        }// while
        // append the number of bits in the file as a 64 bit int
        // convert to Big Endian if Little Endian
        if(state->endianness)
            M.s[7] = lilEndianToBig(state->nobits);
        else
            M.s[7] = state->nobits;
        // set the flag to FINISH
        state->S = FINISH;
    }
    // if we have read more than 56 bytes but less than 64 bytes,
    // there is not enough space to append 1 + 0* + 64bit total
    // an extra message block of padding will be needed here
    else if (nobytes < 64) {
        // set the flag
        state->S = PAD0;
        // append the 1 to the message
        M.e[nobytes] = 0x80;
        // pad the rest of the message block with '0' bits
        while(nobytes < 64) {
            nobytes = nobytes +1;
            M.e[nobytes] = 0x00;
        }// while
    }
    // if the file byte size is a multiple of 64(512bits)
    // a '1' followed by 447 '0' bits followed by the 64bit total
    // we need to create a new message block containing,
    else if(feof(state->file)) {
        // set the flag
        state->S = PAD1;
    }// else if

    // if PAD0 flag is set, append '0' bits then 64 bit total
    if(state->S == PAD0 || state->S == PAD1) {
        for(i = 0; i < 56; i++)
            M.e[i] = 0x00;
        // append the number of bits in the file as a 64 bit int
        // convert to Big Endian if Little Endian
        if(state->endianness)
            M.s[7] = lilEndianToBig(state->nobits);
        else
            M.s[7] = state->nobits;
        // set the flag to FINISH
        state->S = FINISH;
    }// if
                                                        
    // if the PAD1 flag is set
    if(state->S == PAD1) {
        for(i = 0; i < 56; i++)
            M.e[i] = 0x00;
        // append the number of bits in the file as a 64 bit int
        // convert to Big Endian if Little Endian
        if(state->endianness)
            M.s[7] = lilEndianToBig(state->nobits);
        else
            M.s[7] = state->nobits;
        // set the 0th bit of this message block to '1'
        M.e[0] = 0x80;
        // set the flag to FINISH
        state->S = FINISH;
    }// if 
    
    // set the state message block
    for(i = 0; i < 16; i++)
        state->M[i] = M.t[i];     
    
    if(state->verbose && state->S != 3)
        printf("\nCurrent State: %d (0:READ 1:PAD0 2:PAD1 3:FINISH)\n", state->S);
    
    // return false(0) to continue processing
    return 0;
}// nextMsgBlock

// function to determine the system+compiler endianness
// adapted from: http://cs-fundamentals.com/tech-interview/c/c-program-to-check-little-and-big-endian-architecture.php
int endianness() {
    uint8_t x = 1;
    
    char *c = (char*)&x;

    return((int)*c);
}// endianness

// function to initialize the state struct
void initializeState(struct buffer_state * state, int argc, char*argv[]) {
    int i;    
    state->verbose = 0;
    // checking command line arguments
    for(i = 0; i<argc; ++i) {
        if (strcmp("-v", argv[i]) == 0) {
            // set the verbose option
            state->verbose = 1;
        }// if
    }// for   

    // initialize the struct variables
    for(i = 0; i < 16; i++){
        // set all to 0
        state->M[i] = 0;
    }// for
    // calculate the system+compiler endianness
    state->endianness = endianness();
    // set the number of bits read to 0
    state->nobits = 0;
    // set the initial state flag
    state->S = READ;
    // set the file pointer to 0
    state->filePointer = 0;
    
    if(state->verbose && state->endianness)
        printf("Little Endian System+Compiler detected.\n\n");
    else if(state->verbose && state->endianness)
        printf("Big Endian System+Compiler detected.\n\n");
}// initializeState

