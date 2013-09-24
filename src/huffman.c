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

    ht->list[i].leaf->bitstring = NULL;
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

/* Imprime as ocorrências de todos os simbolos em ordem de simbolo */
int print_huffman_list(huffman_list_t *list) {
  size_t i;
  huffman_leaf_t *leaf;

  for(i=0; i<256; i++) {
    leaf = list[i].leaf;
    printf("%5d",leaf->occurrence);
    if (((i+1)%16) == 0)
      printf("\n");
  }
}

int print_huffman_list_occ(huffman_tree_t *ht) {
  huffman_list_t *list;
  int i=0;
  for(list=ht->smallest; list!=NULL; list=list->bigger) {
    printf("0x%02X:%-5d ",list->leaf->symbol,list->leaf->occurrence);
    i++;
    if (((i)%16) == 0)
      printf("\n");
  }
}

int assign_new_node(huffman_node_t **node, huffman_list_t *list) {
  if(list->node == NULL) {
  /* Se o nodo da lista não existir, precisa alocar e copiar tudo */
  /* Isso ocorre quando o elemento na lista for uma folha */
	*node = (huffman_node_t *) malloc(sizeof(huffman_node_t));
	if(*node == NULL)
	  return -1;

    (*node)->n[0] = NULL;
    (*node)->n[1] = NULL;

    (*node)->prefix = NULL;

    (*node)->occurrence = list->leaf->occurrence;

    (*node)->leaf = list->leaf;

  } else {
  /* Se o nodo já estiver alocado, só precisa copiar ele da lista */
  /* Isso ocorre quando o elemento já for um nodo da árvore */
    *node = list->node;
  }

  return 0;
}

void print_huffman_node(huffman_node_t *node, int depth) {
  int i;

  if(node) {
    if(node->n[0]) {
	  for(i=0;i<depth;i++) printf("|");
	  printf("Node   :%-5d\n",node->occurrence);
	  print_huffman_node(node->n[0],depth+1);
	  print_huffman_node(node->n[1],depth+1);
	} else {
	  for(i=0;i<depth;i++) printf("|");
      if(node->leaf->bitstring)
        printf("Leaf:%02X:%-5d %s\n",node->leaf->symbol,node->occurrence,
              node->leaf->bitstring);
      else
        printf("Leaf:%02X:%-5d\n",node->leaf->symbol,node->occurrence);
	}
  }
}

void print_huffman_tree(huffman_tree_t *ht) {
  huffman_list_t *list;

  for(list=ht->smallest; list!=NULL; list=list->bigger) {
    if(list->node)
      print_huffman_node(list->node,0);
    else {
	  printf("Leaf:%02X:%-5d\n",list->leaf->symbol,list->occurrence);
	}
  }
}

void create_bitstrings(huffman_node_t *node, const char *prefix, int depth) {
  int i;

  if(node) {
//    node->prefix = prefix;
    if(node->n[0]) {
    /* Se for um nodo, continua recursivamente */
      char *new_prefix0,*new_prefix1;
      size_t len=(strlen(prefix)+1);

      /* Aloca uma nova string de bits */
      new_prefix0 = malloc(len);
      new_prefix1 = malloc(len);

      /* Junta ao fim da string um bit */
      sprintf(new_prefix0,"%s0",prefix);
      sprintf(new_prefix1,"%s1",prefix);

//	  printf("%sNode   :%-5d\n",prefix,node->occurrence);
	  create_bitstrings(node->n[0],new_prefix0,depth+1);
	  create_bitstrings(node->n[1],new_prefix1,depth+1);
	} else {
//      node->leaf->bitstring = prefix;
//      printf("%sLeaf:%02X:%-5d\n",prefix,node->leaf->symbol,node->occurrence);
	}
  }
}

int build_huffman_tree(huffman_tree_t *ht) {
  huffman_node_t *node;
  huffman_list_t *second_smallest;

  /* Itera até a lista se transformar totalmente numa árvore */
  for (;;) {
    node = (huffman_node_t *) malloc(sizeof(huffman_node_t *));
    if(node == NULL)
      return -1;

    /* Guarda o símbolo com a segunda menor probabilidade na lista */
    second_smallest = ht->smallest->bigger;

    /* Se a lista não contém o segundo menor, é porque todos os 
         elementos estão na árvore, logo o algorítmo pode parar */
    if(second_smallest == NULL) {
      break;
    }

    /* Atribui os dois nodos com menor probabilidade às duas folhas do 
         novo nodo */
    assign_new_node(node->n+0, ht->smallest);
    assign_new_node(node->n+1, second_smallest);

    /* Reusa o ponteiro do segundo menor nodo, mas não apaga a folha */
    /* A lista ainda será útil após para encontrar a string de bits */
    second_smallest->smaller = NULL;
    second_smallest->node = node;
//    second_smallest->leaf = NULL;

    node->occurrence = node->n[0]->occurrence + node->n[1]->occurrence;
    second_smallest->occurrence = node->occurrence;

    /* Novo nodo é colocado temporariamente no início da lista */
    ht->smallest = second_smallest;

    /* Avança o novo nodo na lista caso a soma das ocorrências for maior */
    adjust_symbol_in_list(ht,second_smallest);
  }

  /* Ajusta o nodo raíz da árvore */
  ht->root = ht->smallest->node;

  char *prefix;

  /* Prefixo raíz é uma string nula */
  prefix = malloc(sizeof(char));
  prefix = 0;

//  print_huffman_tree(ht);

  create_bitstrings(ht->root,"",0);

//  print_huffman_tree(ht);

  return 0;
}

int create_huffman_tree(huffman_tree_t *ht, uint8_t *input, size_t len) {
  if(create_huffman_list(ht, input, len))
    return -1;

  if(build_huffman_tree(ht))
    return -1;

  return 0;
}
