#include <stdio.h>
#include <stdint.h>

union msgblock {
    uint8_t e[64];
    uint32_t t[16];
    uint64_t s[8];

};

enum status {READ, PAD0, PAD1, FINISH};



uint32_t sig0(uint32_t x);
uint32_t sig1(uint32_t x);

uint32_t rotr(uint32_t n, uint32_t x);
uint32_t shr(uint32_t n, uint32_t x);

uint32_t SIG0 (uint32_t x);
uint32_t SIG1 (uint32_t x);

uint32_t Ch (uint32_t x, uint32_t y, uint32_t z);
uint32_t Maj (uint32_t x, uint32_t y, uint32_t z);

void sha256(FILE *f);

int nextmsgblock(FILE *f, union msgblock *M, enum status *S, uint64_t *nobits);
int main (int argc, char *argv[])
{

 FILE* f;
 f = fopen(argv[1], "r");

sha256(f);

fclose(f);

return 0;
}

void sha256(FILE *f){

    union msgblock M; 

    uint16_t nobits = 0; 
    uint64_t nobytes; 

    enum status S = READ;

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

     


    int i, t;

    while (nextmsgblock(f, &M, &S, &nobits)) {

    for (t =0; t < 16; t++)
        W[t] = M.t[t];

    for (t = 16; t < 64; t++)
        W[t] = sig1 (W[t-2]) + W[t-7] + sig0(W[t-15]) + W[t-16]; 

    // intialise a b c d and e 
    a = H[0]; b = H[1]; c = H[2]; d = H[3]; 
    e = H[4]; f = H[5]; g = H[6]; h = H[7];

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

    }
    // step 4
    H[0] = a + H[0];
    H[1] = b + H[1];
    H[2] = c + H[2];
    H[3] = d + H[3];
    H[4] = e + H[4];
    H[5] = f + H[5];
    H[6] = g + H[6];
    H[7] = h + H[7];
    }
    printf("%x %x %x %x %x %x %x %x\n", H[0], H[1], H[2], H[3], H[4], H[5], H[6], H[7]);
}

uint32_t rotr(uint32_t n, uint32_t x){
    return (x >> n) | (x << (32 - n));
}

uint32_t shr(uint32_t n, uint32_t x){
    return (x >> n);
}


uint32_t sig0(uint32_t x){
    return (rotr(7, x) ^ rotr(18, x) ^ shr(3, x));
}
uint32_t sig1(uint32_t x){
    return (rotr(17, x) ^ rotr(19, x) ^ shr(10, x));
}

uint32_t SIG0 (uint32_t x){
   return (rotr(2, x) ^ rotr(13, x) ^ rotr(22, x));

}
uint32_t SIG1 (uint32_t x){
    return (rotr(6, x) ^ rotr(11, x) ^ rotr(25, x));
}

uint32_t Ch (uint32_t x, uint32_t y, uint32_t z){
    return ((x & y) ^ ((!x) & z));
}
uint32_t Maj (uint32_t x, uint32_t y, uint32_t z){
    return ((x & y) ^ (x & z) ^ (y & z));
}

int nextmsgblock(FILE *f, union msgblock *M, enum status *S, uint64_t *nobits) {
    

    // Number of bytes from fread
    uint64_t nobytes; 

    

    
    // for looping
    int i; 

if (*S == FINISH)
    return 0;

    if (*S == PAD0 || *S == PAD1) {
        for (i = 0; i < 56; i++) 
            M->e[i] = 0x80;
        M->s[7] = *nobits;
        *S = FINISH; 

        if (S == PAD1) 
        M->e[0] = 0x80;

        return 1; 
    }
    

    while (S == READ) {
    nobytes = fread(M->e, 1, 64, f);
    
    *nobits = *nobits + (nobytes * 8);
    if (nobytes < 56) {
        M->e[nobytes] = 0x80;
        while (nobytes < 56){
            nobytes = nobytes + 1; 
            M->e[nobytes] = 0x00;
        }
        M->s[7] = nobits;
        *S = FINISH; 
    } else if (nobytes < 64) {
        *S = PAD0;
        M->e[nobytes] = 0x80;
        while (nobytes < 64) {
            nobytes = nobytes +1;
            M->e[nobytes] = 0x00;

        }
    }   else if (feof(f)) {
            *S = PAD1;
    }
 
 return 1;
  
}



