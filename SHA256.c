#include <stdio.h>
#include <stdint.h>

union msgblock {
    uint8_t e[64];
    uint32_t t[16];
    uint64_t s[8];

};
// keeps track of state of pad file
enum status {READ, PAD0, PAD1, FINISH};


// see section 4.1.2
// all the various declerations
uint32_t sig0(uint32_t x);
uint32_t sig1(uint32_t x);

uint32_t rotr(uint32_t a, uint32_t b);
uint32_t shr(uint32_t n, uint32_t x);

uint32_t SIG0 (uint32_t x);
uint32_t SIG1 (uint32_t x);

uint32_t Ch (uint32_t x, uint32_t y, uint32_t z);
uint32_t Maj (uint32_t x, uint32_t y, uint32_t z);
// this was used to attempt to work with big endian, so far I have no luck with it
#define SWAP_UINT32(x)  (((x) >> 24) | (((x)&0x00FF0000) >> 8) | (((x)&0x0000FF00) << 8) | ((x) << 24))

#define SWAP_UINT64(x)                                                      \
	((((x) >> 56) & 0x00000000000000FF) | (((x) >> 40) & 0x000000000000FF00) | \
	 (((x) >> 24) & 0x0000000000FF0000) | (((x) >> 8) & 0x00000000FF000000) |  \
	 (((x) << 8) & 0x000000FF00000000) | (((x) << 24) & 0x0000FF0000000000) |  \
	 (((x) << 40) & 0x00FF000000000000) | (((x) << 56) & 0xFF00000000000000))

#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)

void sha256(FILE *msgf);

int nextmsgblock(FILE *msgf, union msgblock *M, enum status *S, uint64_t *nobits);
int main (int argc, char *argv[])
{

 FILE* msgf;
 msgf = fopen(argv[1], "r");

sha256(msgf);

fclose(msgf);

return 0;
}

void sha256(FILE *msgf){
    // defines the instance of the current message block
    union msgblock M; 

    uint64_t nobits = 0; 
    uint64_t nobytes; 
    // sets the staus to read for the message block 
    enum status S = READ;
    // the k constants that were included with the sha 256 document
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
    // see section 6.2
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

     

    // for looping
    int i = 0, t;
    // this iterates through the message blocks
    while (nextmsgblock(msgf, &M, &S, &nobits)) {
        
    for (t =0; t < 16; t++)
    // this if statement is attempting the converstion for big endian, since I cannot get the right hash in general I am unsure or not if it works
        if (IS_BIG_ENDIAN) 
			{
				W[t] = M.t[t];
			}
			else
			{
				
				W[t] = SWAP_UINT32(M.t[t]);
			}

    for (t = 16; t < 64; t++)
       W[t] = sig1(W[t - 2]) + W[t - 7] + sig0(W[t - 15]) + W[t - 16];

    // intialise a b c d and e 
    a = H[0]; b = H[1]; c = H[2]; d = H[3]; 
    e = H[4]; f = H[5]; g = H[6]; h = H[7];
    // step 3
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
    H[0] = (a + H[0]);
    H[1] = (b + H[1]);
	H[2] = (c + H[2]);
	H[3] = (d + H[3]);
	H[4] = (e + H[4]);
	H[5] = (f + H[5]);
	H[6] = (g + H[6]);
	H[7] = (h + H[7]);
    }
    // prints out the values
    printf("%08x%08x%08x%08x%08x%08x%08x%08x\n", H[0], H[1], H[2], H[3], H[4], H[5], H[6], H[7]);
}
// bit operations

// rotates the bit x by a places, places overhanging bits back to start
uint32_t rotr(uint32_t a, uint32_t b){
    return (a >> b) | (a << (32 - (b)));
}
// shifts the bit by x and b places
uint32_t shr(uint32_t x, uint32_t b)
{
	return (x >> b);
}


// rotates it right with 7 xor
uint32_t sig0(uint32_t x){
    return (rotr(x, 7) ^ rotr(x, 18) ^ shr(x, 3));
}
uint32_t sig1(uint32_t x){
    return (rotr(x,17) ^ rotr(x,19) ^ shr(x, 10));
}

uint32_t SIG0 (uint32_t x){
   return (rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22));

}
uint32_t SIG1 (uint32_t x){
    return (rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25));
}
// x input chooses if the output is from y or z
uint32_t Ch (uint32_t x, uint32_t y, uint32_t z){
    return ((x & y) ^ (~(x)&z));
}
// meaning majority, for each bit index the result is according to the majority of 3 input bits 
uint32_t Maj (uint32_t x, uint32_t y, uint32_t z){
    return ((x & y) ^ (x & z) ^ (y & z));

}

int nextmsgblock(FILE *msgf, union msgblock *M, enum status *S, uint64_t *nobits) {
    

    // Number of bytes from fread
    uint64_t nobytes; 

    

    
    // for looping
    int i; 
// checks to see if all message blocks have finished
if (*S == FINISH){
    return 0;
}   // if statement used for selecting which padding to use
    if (*S == PAD0 || *S == PAD1) {
        // sets first 56 bytes to all zero bits
        for (i = 0; i < 56; i++) {
            M->e[i] = 0x80;
        }
        // this was used to try and get big endian working, should set the last 64 bits to number of bits in the file
        M->s[7] = SWAP_UINT64 (*nobits);
        // sets state to finish
        *S = FINISH; 

        if (*S == PAD1){
        M->e[0] = 0x80;
        }
        return 1; 
    }
    // reads 64 bytes at a time from the file 
    nobytes = fread(M->e, 1, 64, msgf);
    // keeps track of bits read
    *nobits = *nobits + (nobytes * 8);
    // if 56 bytes have been read, put all padding in this message block
    if (nobytes < 56) {
        M->e[nobytes] = 0x80;
        while (nobytes < 56){
            nobytes = nobytes + 1; 
            M->e[nobytes] = 0x00;
        }
        M->s[7] = SWAP_UINT64(*nobits);
        *S = FINISH; 
    } else if (nobytes < 64) {
        *S = PAD0;
        M->e[nobytes] = 0x80;
        while (nobytes < 64) {
            nobytes = nobytes +1;
            M->e[nobytes] = 0x00;

        }
    }   else if (feof(msgf)) {
            *S = PAD1;
    }
 
 return 1;
  
}




