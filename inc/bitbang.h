#ifndef __BITBANG_H__
#define __BITBANG_H__

#include <stdio.h>
#include <stdint.h>

typedef struct {
  FILE *file;
  uint8_t last_byte;
  uint8_t byte_pos;
} bitwrite_t;

int create_bitwrite(bitwrite_t *bw, const char *filename);

int write_bit(bitwrite_t *bw, const char bit);

int write_bitstring(bitwrite_t *bw, const char *bitstring);

int close_bitwrite(bitwrite_t *bw);

#endif /* __BITBANG_H__ */
