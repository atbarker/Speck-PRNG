#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <syscall.h>

//block size in bytes, key size is assumed to be the same
#define BLOCK_SIZE 16

#define ROR(x, r) ((x >> r) | (x << (64 - r)))
#define ROL(x, r) ((x << r) | (x >> (64 - r)))
#define R(x, y, k) (x = ROR(x, 8), x += y, x ^= k, y = ROL(y, 3), y ^= x)
#define ROUNDS 32

/**
 * pt: plaintext
 * ct: ciphertext
 * k: key
 * we assume that input arrays are of length 2 so we get 128 bits
 * Should generate the key on the fly, just for simplicity sake
 * Better performance can be had by computing round keys once.
 * This function is obtained from the following paper, https://eprint.iacr.org/2013/404
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
 * output_length: size of the output block
 * output_block: destination for pseudorandom bits
 * seed: a 128 bit random number
 * Generate a block of random bytes given a key (seed) by running speck in counter mode
 * block input should be aligned to 128 bit (16 byte)  boundaries
 * We assume a length of 16 bytes (128 bits) for the seed.
 */
void generate_block_ctr(size_t output_length, uint8_t *output_block, uint8_t *seed){
    uint32_t rounds = output_length/BLOCK_SIZE;
    uint64_t i, ctr[2], key[2], output[2];
    uint64_t j = 0;

    if(output_length % BLOCK_SIZE != 0){
        printf("Not aligned to 128 bit boundary\n");
    }

    key[0] = ((uint64_t *)seed)[0];
    key[1] = ((uint64_t *)seed)[1];

    ctr[0] = 0;
    ctr[1] = 0;
    
    for(i = 0; i < rounds; i++){
       speck_encrypt(output, ctr, key);
       ((uint64_t *)output_block)[j + 1] = output[1];
       ((uint64_t *)output_block)[j + 0] = output[0];
       ctr[0]++;
       if(ctr[0] == 0) ctr[1]++;
       j += 2;
    }
}

/**
 * Get a random 128 bit number as our key/seed
 * This must be from a cryptographically secure RNG
 */
uint64_t * get_seed_64(){
    static uint64_t random[2];
    syscall(SYS_getrandom, random, 16, 0);
    return random;
}

void speck_128_hash(uint8_t *data, size_t data_length, uint8_t* hash){
    uint64_t seed[2] = {0,0};
    uint32_t rounds = data_length/BLOCK_SIZE;
    uint64_t i, j, ctr[2], temp[2];
    
    j = 0;
    ctr[0] = 0;
    ctr[1] = 0;
    memset(hash, 0, 16);

    for(i = 0; i < rounds; i++){
	temp[0] = ((uint64_t *)data)[j + 0];
        temp[1] = ((uint64_t *)data)[j + 1]; 	
        speck_encrypt(temp, ctr, seed);
        ((uint64_t*)hash)[0] ^= temp[0];
        ((uint64_t*)hash)[1] ^= temp[1];
        ctr[0]++;
        if(ctr[0] == 0) ctr[1]++;	
	j += 2;
    }
}
