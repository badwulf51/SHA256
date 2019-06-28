#include <stdio.h>
#include <stdint.h>

void sha256();

int main (int argc, char *argv[])
{
sha256();

return 0;
}

void sha256(){

    uint32_t W[64];
    uint32_t a, b, c, d, e, f, g, h;

    uint32_t T1, T2;

    uint32_t H[8] = {
        0x6a09e667, 
        0xbb67ae85, 
        0x3c6ef372, 
        0xa54ff53a,
        0x510e527f, 
        0x9b05688c, 
        0x1f83d9ab,
        0x5be0cd19
    };

    uint32_t M[16];  
}