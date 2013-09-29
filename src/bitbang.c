#include "bitbang.h"

int create_bitwrite(bitwrite_t *bw, const char *filename) {
  bw->file = fopen(filename,"wb");

  if(bw->file == NULL) {
    fprintf(stderr,"Não foi possível abrir %s\n",filename);
    return -1;
  }

  bw->last_byte = 0;
  bw->byte_pos = 0;

  return 0;
}

int write_bit(bitwrite_t *bw, char bit) {
  switch(bit) {
    case '0':
      bw->last_byte &= 255 - (1 << bw->byte_pos);
      break;
    case '1':
      bw->last_byte |= (1 << bw->byte_pos);
      break;
    default:
      fprintf(stderr,"Bit inválido: %c\n",bit);
      return -1;
  }

  bw->byte_pos++;

  /* Se já completou um byte, escreve ele no arquivo */
  if(bw->byte_pos == 8) {
    size_t res;
    res = fwrite(&(bw->last_byte),1,1,bw->file);
    if(res == 0) {
      bw->byte_pos--;
      fprintf(stderr,"Não foi possível escrever no arquivo\n");
      return -2;
    } else {
      bw->last_byte = 0;
      bw->byte_pos = 0;
    }
  }
  return 0;
}

int write_bitstring(bitwrite_t *bw, const char *bitstring) {
  int i=0;
  while(bitstring[i] != 0) {
    int res = write_bit(bw,bitstring[i]);
    if(res) {
      return res;
    }
    i++;
  }
  return 0;
}

int close_bitwrite(bitwrite_t *bw) {
  int i;
  for(i=0; i<8; i++) {
    if(bw->byte_pos == 0) {
      fclose(bw->file);
      bw->file = NULL;
      return 0;
    } else {
      write_bit(bw,'0');
    }
  }
  fprintf(stderr,"Não foi possível fechar o bitwrite");
  return -1;
}
