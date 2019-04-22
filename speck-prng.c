#include <stdint.h>
#include <stdio.h>

//block size in bytes
#define BLOCK_SIZE 16
#define KEY_SIZE 16

/**
 * Initial encrypt function obtained from wikipedia 
 */

#define ROR(x, r) ((x >> r) | (x << (64 - r)))
#define ROL(x, r) ((x << r) | (x >> (64 - r)))
#define R(x, y, k) (x = ROR(x, 8), x += y, x ^= k, y = ROL(y, 3), y ^= x)
#define ROUNDS 32

/**
 *pt: plaintext
 *ct: ciphertext
 *k: key
 *we assume that input arrays are of length 2 so we get 128 bit numbers
 *Should generate the key on the fly, just for simplicity sake
 */
void speck_encrypt(uint64_t ct[2], uint64_t const pt[2], uint64_t const K[2])
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

/**
 * Generate a block of random bytes given a key (seed) by running speck in counter mode
 * block input should be aligned to 128 bit (16 byte)  boundaries
 * We assume a length of 2 for the seed.
 */
void generate_block_ctr(size_t output_length, uint8_t *output_block, uint8_t *seed){
    int rounds = output_length/BLOCK_SIZE;
    uint64_t i, nonce[2], key[2], key[68], x, y, j;
    uint8_t *block = malloc(16);

    if(output_length % BLOCK_SIZE != 0){
        printf("Not aligned to 128 bit boundary\n");
    }
    
    for(i = 0; i < rounds; i++){
       x = nonce[1]; y=nonce[0]; nonce[0]++;
       speck_encrypt(&x, &y, key);
       ((uint64_t *)output_block)[j + 1] = x;
       ((uint64_t *)output_block)[j + 0] = y;
       j += 2;
    }

    free(block);
}

/**
 * Get a random 128 bit number as our key/seed
 * This must be from a cryptographically secure RNG
 */
uint64_t* get_seed_64(){

}
