#include "huffman.h"

int create_huffman_tree(huffman_tree_t *ht, uint8_t *input, size_t len) {
  size_t i;

  /* Cria uma lista de onde será feita a árvore de Huffman */
  ht->list = (huffman_list_t *) malloc(sizeof(huffman_list_t)*256);

  ht->num_symbols = len;

  if (ht->list == NULL)
    return -1;

  for(i=0;i<256;i++) {
    ht->list[i].leaf.symbol = i;
    ht->list[i].leaf.prob = 0.0;
    ht->list[i].leaf.occurrence = 0;

    ht->list[i].smaller = &(ht->list[i-1]);
    ht->list[i].bigger  = &(ht->list[i+1]);
  }

  ht->list[0].smaller = NULL;
  ht->list[255].bigger= NULL;

  for(i=0;i<len;i++) {
    ht->list[input[i]].leaf.occurrence++;

    /* Avançar na lista caso o símbolo ocorrer mais vezes */
	if(ht->list[input[i]].bigger != NULL) {
      while(ht->list[input[i]].bigger->leaf.occurrence < ht->list[input[i]].leaf.occurrence) {
	    huffman_list_t *bigger_ptr;

	    bigger_ptr = ht->list[input[i]].bigger;

		/* If bigger is NULL, then it is already the max */
		if(bigger_ptr != NULL) {
          ht->list[input[i]].bigger = bigger_ptr->bigger;
          ht->list[input[i]].smaller = bigger_ptr;

          bigger_ptr->bigger = &(ht->list[input[i]]);
	      bigger_ptr->smaller= ht->list[input[i]].smaller;
        }

		/* Quebra a iteração antes que ele tente ler denovo o estado da variável */
		if(ht->list[input[i]].bigger == NULL) {
		  break;
		}
	  }
    }
  }

  return 0;
}
