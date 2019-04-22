#include <stdint.h>

/**
 * Initial encrypt function obtained from wikipedia 
 */

#define ROR(x, r) ((x >> r) | (x << (64 - r)))
#define ROL(x, r) ((x << r) | (x >> (64 - r)))
#define R(x, y, k) (x = ROR(x, 8), x += y, x ^= k, y = ROL(y, 3), y ^= x)
#define ROUNDS 32


//pt: plaintext
//ct: ciphertext
//k: key
void encrypt(uint64_t ct[2], uint64_t const pt[2], uint64_t const K[2])
{
   uint64_t y = pt[0], x = pt[1], b = K[0], a = K[1];

   R(x, y, b);
   for (int i = 0; i < ROUNDS - 1; i++) {
      R(a, b, i);
      R(x, y, b);
   }

   ct[0] = y;
   ct[1] = x;
}

void generate_block(size_t length, uint8_t *block, uint64_t seed){

}

uint64_t get_seed_64(){

}
