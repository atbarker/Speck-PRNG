#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "speck-prng.h"
#include <time.h>
#include <string.h>

#define BLOCK_SIZE 4096

void hexDump (char *desc, void *addr, uint32_t len) {
    uint32_t i;
    uint8_t buff[17];
    uint8_t *pc = (uint8_t*)addr;

    // Output description if given.
    if (desc != NULL)
        printf ("%s:\n", desc);

    if (len == 0) {
        printf("  ZERO LENGTH\n");
        return;
    }

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}


int main(){
    uint8_t *key = (uint8_t *)get_seed_64();
    uint8_t *block = malloc(BLOCK_SIZE);
    struct timespec start, end;
    uint8_t hash[16];


    clock_gettime(CLOCK_MONOTONIC, &start);
    generate_block_ctr(BLOCK_SIZE, block, key);
    clock_gettime(CLOCK_MONOTONIC, &end);

    printf("Time to generate random bytes in ns: %ld\n", end.tv_nsec - start.tv_nsec); 

    //hexDump("random data", block, BLOCK_SIZE);
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    speck_128_hash(block, BLOCK_SIZE, hash);
    clock_gettime(CLOCK_MONOTONIC, &end);

    hexDump("hash of random", hash, 16);
    speck_128_hash(block, BLOCK_SIZE, hash);
    hexDump("hash of random 2", hash, 16);
    printf("Time to hash in ns: %ld\n", end.tv_nsec - start.tv_nsec);

    free(block);
    return 0;
}
