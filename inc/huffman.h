#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include "rev.h"
#include "debug.h"
#include "bitbang.h"

#include <string.h>

typedef struct {
  double prob;
  uint32_t occurrence;
  uint8_t symbol;
  const char *bitstring;
} huffman_leaf_t;

typedef struct huffman_node_s {
  struct huffman_node_s *n[2];

  uint32_t occurrence;

  huffman_leaf_t *leaf;

  const char *prefix;
} huffman_node_t;

typedef struct huffman_list_s {
  struct huffman_list_s *smaller;
  struct huffman_list_s *bigger;

  uint32_t occurrence;

  huffman_node_t *node;

  huffman_leaf_t *leaf;
} huffman_list_t;

typedef struct {
  huffman_node_t *n;

  huffman_list_t *list;

  huffman_list_t *smallest;
  huffman_list_t *biggest;

  huffman_node_t *root;

  uint32_t num_symbols;
} huffman_tree_t;

int create_huffman_tree(huffman_tree_t *ht, uint8_t *input, size_t len);

int write_huffman_tree_to_file(huffman_node_t *node, bitwrite_t *bw);

#endif /* __HUFFMAN_H__ */
