#ifndef __BITBANG_H__
#define __BITBANG_H__

#include <stdio.h>
#include <stdint.h>

typedef struct {
  FILE *file;
  uint8_t last_byte;
  uint8_t byte_pos;
} bitwrite_t;

typedef struct {
  FILE *file;
  uint8_t last_byte;
  uint8_t byte_pos;
} bitread_t;

int create_bitwrite(bitwrite_t *bw, FILE *file);

int write_bit(bitwrite_t *bw, const char bit);

int write_8bits(bitwrite_t *bw, uint8_t byte);

int write_bitstring(bitwrite_t *bw, const char *bitstring);

int close_bitwrite(bitwrite_t *bw);


int create_bitread(bitread_t *br, FILE *file);

int read_bit(bitread_t *br, char *bit);

int read_8bits(bitread_t *br, uint8_t *byte);

#endif /* __BITBANG_H__ */
