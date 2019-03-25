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

    uint64_t nobytes;

    FILE* file;
    file = fopen(argv[1], "r");

    // need to preform some error chech on file opening
    // f_error/f_err maybe

    while(!feof(f)) {
        nobytes = fread(M.e, 1, 64, f);
        printf("%llu\n", nobytes);
    }// while

    fclose(f);

    return 0;
}// main
