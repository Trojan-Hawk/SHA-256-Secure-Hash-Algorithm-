// Timothy Cassidy
// G00333333
// link to SHA standard: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf

// The usual input/output header file
#include <stdio.h>
// For using fixed bit length integers
#include <stdint.h>

void sha256();

// macro functions
// see section 3.2 for definitions
#define ROTR(x,n)  ((x << (32 - n)) | (x >> n))
#define ROTL(x,n)  ((x << n) | (x >> (32 - n)))
// see section 4.1.2 for definitions
#define sig0(x)    (ROTR(7, x) ^ ROTR(18, x) ^ SHR(3, x))
#define sig1(x)    (ROTR(17, x) ^ ROTR(19, x) ^ SHR(10, x))
#define SHR(x,n)   (x >> n)
#define Ch(x,y,z)  ((x & y) ^ (!(x) & z))// choosing  
#define Maj(x,y,z) ((x & y) ^ (x & z) ^ (y & z))// majority vote
#define SIG0(x)    (ROTR(2, x) ^ ROTR(13, x) ^ ROTR(22, x))
#define SIG1(x)    (ROTR(6, x) ^ ROTR(11, x) ^ ROTR(25, x))

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
    };

    // the current message block
    uint32_t M[16];

    // for looping
    int i, t;

    // loop through the message blocks
    for(i = 0; i < 1; i++){

        // from page 22, W[t] = M[t] for 0 <= t <= 15
        for(t = 0; t < 16; t++){
            W[t] = M[t];
        }// for

        // from page 22, W[t] = ...
        for (t = 16; t < 64; t++){
            W[t] = sig1(W[t-2]) + W[t-7] + sig0(W[t-15]) + W[t-16];
        }// for

        // initilize a...h as per step 2, page 22
        a = H[0]; b = H[1]; c = H[2]; d = H[3];
        e = H[4]; f = H[5]; g = H[6]; h = H[7];
    
        // see section 3.2 for definitions
        for (t = 0; t < 64; t ++){
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

    }// for

}// sha25





