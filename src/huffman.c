#include "huffman.h"

void adjust_symbol_in_list(huffman_tree_t *ht, huffman_list_t *current) {

  /* Avançar na lista caso o símbolo ocorrer mais vezes que o próximo */
  while(current->bigger != NULL) {
    /* Testa se simbolo adiante na lista ocorre menas vezes */
    if(current->bigger->occurrence < current->occurrence) {
      huffman_list_t *bigger_ptr;
      huffman_list_t *smaller_ptr;

      /* Usa estas variáveis para simplificar o código */
      bigger_ptr  = current->bigger;
      smaller_ptr = current->smaller;

      /* Ajusta o ponteiro do nodo após o maior (se existir) */
      if(bigger_ptr->bigger != NULL)
        bigger_ptr->bigger->smaller = current;
      else
        ht->biggest = current;

      /* Ajusta os ponteiros do nodo atual */
      current->bigger = bigger_ptr->bigger;
      current->smaller = bigger_ptr;

      /* Ajusta os ponteiros do nodo maior */
      bigger_ptr->bigger = current;
      bigger_ptr->smaller= smaller_ptr;

      /* Ajusta o ponteiro do nodo menor (se existir) */
      if(smaller_ptr != NULL)
        smaller_ptr->bigger = bigger_ptr;
      else
        ht->smallest = bigger_ptr;
    } else
      break;
  }
}

int create_huffman_list(huffman_tree_t *ht, uint8_t *input, size_t len) {
  size_t i;
  huffman_leaf_t *leaf_list;

  /* Cria uma lista de onde será feita a árvore de Huffman */
  ht->list = (huffman_list_t *) malloc(sizeof(huffman_list_t)*256);

  if (ht->list == NULL)
    return -1;

  leaf_list = (huffman_leaf_t *) malloc(sizeof(huffman_leaf_t)*256);

  if (leaf_list == NULL)
    return -1;

  ht->num_symbols = len;

  for(i=0;i<256;i++) {
    ht->list[i].leaf = leaf_list+i;

    ht->list[i].leaf->symbol = i;
    ht->list[i].leaf->prob = 0.0;
    ht->list[i].leaf->occurrence = 0;

    ht->list[i].smaller = ht->list+i-1;
    ht->list[i].bigger  = ht->list+i+1;

    ht->list[i].node = NULL;
  }

  ht->list[0].smaller = NULL;
  ht->list[255].bigger= NULL;

  ht->smallest = ht->list;
  ht->biggest = ht->list+255;

  for(i=0;i<len;i++) {
    ht->list[input[i]].occurrence = ++ht->list[input[i]].leaf->occurrence;

    adjust_symbol_in_list(ht,ht->list+input[i]);
  }

  return 0;
}

int assign_new_node(huffman_node_t **node, huffman_list_t *list) {

  if(list->node == NULL) {
	*node = (huffman_node_t *) malloc(sizeof(huffman_node_t *));
	if(*node == NULL)
	  return -1;

    (*node)->n[0] = NULL;
    (*node)->n[1] = NULL;

    (*node)->occurrence = list->leaf->occurrence;

    (*node)->leaf = list->leaf;

  } else {
    *node = list->node;
  }

  printf("bbb%p\n",node[0]);fflush(stdout);
  printf("bbb%p\n",node[0]->leaf);fflush(stdout);

  return 0;
}

void print_huffman_node(huffman_node_t *node, int depth) {
  int i;

  printf("dbg1"); fflush(stdout);

  if(node) {
    printf("dbg%d",__LINE__); fflush(stdout);
    if(node->n[0]) {
	  printf("dbg%d",__LINE__); fflush(stdout);
	  for(i=0;i<depth;i++) printf(" ");
	  printf("Node   :%-5d\n",node->occurrence);
	  print_huffman_node(node->n[0],depth+1);
	  print_huffman_node(node->n[1],depth+1);
	} else {
    printf("dbg%d\n",__LINE__); fflush(stdout);
    printf("ptr%p",node->leaf); fflush(stdout);
	  for(i=0;i<depth;i++) printf(" ");
	  printf("Leaf:%3d%-5d\n",node->leaf->symbol,node->occurrence);
	}
  } else {
    printf("dbg%d",__LINE__); fflush(stdout);
    for(i=0;i<depth;i++) printf(" ");
	printf("Leaf:%3d%-5d\n",node->leaf->symbol,node->occurrence);
  }
}

void print_huffman_tree(huffman_tree_t *ht) {
  huffman_list_t *list;

  for(list=ht->smallest; list!=NULL; list=list->bigger) {
    if(list->node)
      print_huffman_node(list->node,0);
    else {
	  printf("Leaf%3d:%-5d\n",list->leaf->symbol,list->occurrence);
	}
  }
}

int build_huffman_tree(huffman_tree_t *ht) {
  huffman_node_t *node;
  huffman_list_t *second_smallest;

  do {
    node = (huffman_node_t *) malloc(sizeof(huffman_node_t *));
    if(node == NULL)
      return -1;

    second_smallest = ht->smallest->bigger;

    assign_new_node(node->n+0, ht->smallest);
    assign_new_node(node->n+1, second_smallest);

    printf("aaa%p\n",node->n[0]);fflush(stdout);
    printf("aaa%p\n",node->n[0]->leaf);fflush(stdout);
    second_smallest->smaller = NULL;
    second_smallest->node = node;
    second_smallest->leaf = NULL;

    node->occurrence = node->n[0]->occurrence + node->n[1]->occurrence;
    second_smallest->occurrence = node->occurrence;

    /* Falta ajustar a posiçao na lista */
    ht->smallest = second_smallest;

    adjust_symbol_in_list(ht,second_smallest);

	print_huffman_tree(ht);
    break;
  } while(1);

  return 0;
}

int create_huffman_tree(huffman_tree_t *ht, uint8_t *input, size_t len) {
  if(create_huffman_list(ht, input, len))
    return -1;

  if(build_huffman_tree(ht))
    return -1;

  return 0;
}


