#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "speck-prng.h"

#define BLOCK_SIZE 4096

int main(){
    uint8_t *key = get_seed_64();
    uint8_t *block = malloc(4096);

    generate_block_ctr(BLOCK_SIZE, block, key);

    free(key);
    free(block);
    return 0;
}
