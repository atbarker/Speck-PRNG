#ifndef PRNG_H
#define PRNG_H

void generate_block_ctr(size_t length, uint8_t *block, uint8_t *seed);

uint64_t * get_seed_64();

#endif
