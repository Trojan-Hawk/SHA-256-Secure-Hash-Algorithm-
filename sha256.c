// Timothy Cassidy
// G00333333
// link to SHA standard

// The usual input/output header file
#include <stdio.h>
// For using fixed bit length integers
#include <stdint.h>

void sha256();

// macro functions
// see section 3.2 for definitions
#define ROTR(x,n) ((x << (32 - n)) | (x >> n))
#define ROTL(x,n) ((x << n) | (x >> (32 - n)))
// see sections 4.1.2 and 4.2.2 for definitions
#define SIG0(x)   (ROTR(7, x) ^ ROTR(18, x) ^ SHR(3, x))
#define SIG1(x)   (ROTR(17, x) ^ ROTR(19, x) ^ SHR(10, x))
#define SHR(x,n)  (x >> n)

int main(int argc, char *argv[]) {
    
    sha256();

    return 0;
}// main


void sha256() {
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

    // the current message block
    uint32_t M[16];
    // for looping
    int t;
    
    // from page 22, W[t] = M[t] for 0 <= t <= 15
    for(t = 0; t < 16; t++){
        W[t] = M[t];
    }// for

    // from page 22, W[t] = ...
    for (t = 16; t < 64; t++){
        SIG1(W[t-2]) + W[t-7] + SIG0(W[t-15]) + W[t-16];
    }// for

}// sha25

