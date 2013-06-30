#include "huffman.h"

int create_huffman_list(huffman_tree_t *ht, uint8_t *input, size_t len) {
  size_t i;

  for(i=0;i<256;i++) {
    ht->list[i].leaf.symbol = i;
    ht->list[i].leaf.prob = 0.0;
    ht->list[i].leaf.occurrence = 0;

    ht->list[i].smaller = ht->list+i-1;
    ht->list[i].bigger  = ht->list+i+1;
  }

  ht->list[0].smaller = NULL;
  ht->list[255].bigger= NULL;
  ht->biggest = ht->list+255;

  for(i=0;i<len;i++) {
    ht->list[input[i]].leaf.occurrence++;

    /* Avançar na lista caso o símbolo ocorrer mais vezes que o próximo */
    while(ht->list[input[i]].bigger != NULL) {

      if(ht->list[input[i]].bigger->leaf.occurrence < 
              ht->list[input[i]].leaf.occurrence) {
        huffman_list_t *bigger_ptr;
        huffman_list_t *smaller_ptr;
        huffman_list_t *current_ptr;

        /* Usa estas variáveis para simplificar o código */
        current_ptr = ht->list+input[i];
        bigger_ptr  = current_ptr->bigger;
        smaller_ptr = current_ptr->smaller;

        /* Ajusta o ponteiro do nodo após o maior (se existir) */
        if(bigger_ptr->bigger != NULL)
          bigger_ptr->bigger->smaller = current_ptr;
        else 
		  ht->biggest = current_ptr;

        /* Ajusta os ponteiros do nodo atual */
        current_ptr->bigger = bigger_ptr->bigger;
        current_ptr->smaller = bigger_ptr;

        /* Ajusta os ponteiros do nodo maior */
        bigger_ptr->bigger = current_ptr;
        bigger_ptr->smaller= smaller_ptr;

        /* Ajusta o ponteiro do nodo menor (se existir) */
        if(smaller_ptr != NULL)
          smaller_ptr->bigger = bigger_ptr;
      } else
        break;
    }
  }

  return 0;
}

int create_huffman_tree(huffman_tree_t *ht, uint8_t *input, size_t len) {

  /* Cria uma lista de onde será feita a árvore de Huffman */
  ht->list = (huffman_list_t *) malloc(sizeof(huffman_list_t)*256);

  ht->num_symbols = len;

  if (ht->list == NULL)
    return -1;

  create_huffman_list(ht, input, len);

  return 0;
}
