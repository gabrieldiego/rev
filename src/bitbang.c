#include "bitbang.h"

int create_bitwrite(bitwrite_t *bw, FILE *file) {
  bw->file = file;
  bw->last_byte = 0;
  bw->byte_pos = 7;

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

  /* Se já completou um byte, escreve ele no arquivo */
  if(bw->byte_pos == 0) {
    size_t res;
    res = fwrite(&(bw->last_byte),1,1,bw->file);

    if(res == 0) {
      fprintf(stderr,"Não foi possível escrever no arquivo\n");
      return -2;
    } else {
      bw->last_byte = 0;
      bw->byte_pos = 7;
    }
  } else {
    bw->byte_pos--;
  }
  return 0;
}

int write_8bits(bitwrite_t *bw, uint8_t byte) {
  int i;
  for(i=0;i<8;i++) {
    char bit = (byte & 0x80)?'1':'0';
    int res = write_bit(bw,bit);
    if(res) {
      return res;
    }
    byte = byte << 1;
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
    if(bw->byte_pos == 7) {
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

int create_bitread(bitread_t *br, FILE *file) {
  br->file = file;
  br->last_byte = 0;
  br->byte_pos = 0;
}

int read_bit(bitread_t *br, char *bit) {
  if(br->byte_pos == 0) {
    size_t res = fread(&(br->last_byte),1,1,br->file);
    if(res == 0) {
      fprintf(stderr,"Não foi possível ler do arquivo\n");
      return -2;
    } else {
      br->byte_pos = 7;
    }
  }

  *bit = (br->last_byte & (1<<br->byte_pos))?'1':'0';

  br->byte_pos--;
  return 0;
}

int read_8bits(bitread_t *br, uint8_t *byte) {
  char bit;
  int i;
  *byte = 0;

  for(i=0;i<8;i++) {
    if(read_bit(br,&bit)) {
      return -1;
    }
    switch (bit) {
      case '0':
        *byte = (*byte << 1);
        break;
      case '1':
        *byte = (*byte << 1) + 1;
        break;
    }
  }
  return 0;
}
