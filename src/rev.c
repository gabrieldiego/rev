#include "rev.h"

int main(int argc, char **argv) {
  FILE *input_file;
  int32_t w,h;

  if(argc < 4) {
    fprintf(stderr,"Few arguments.\n");
	exit(-1);
  }

  input_file = fopen(argv[1],"rb");

  if(input_file == NULL) {
    fprintf(stderr,"Could not open file %s.\n",argv[1]);
    exit(-1);
  }

  w = atoi(argv[2]);
  h = atoi(argv[3]);

  if(h <= 0 || w <= 0) {
    fprintf(stderr,"Invalid dimensions.\n");
    exit(-1);
  }

  uint32_t i,j;
  uint32_t statistics[256];
  uint32_t statistics_diff[256];
  uint8_t *frame;
  
  frame = malloc(h*w*3/2+1);
  
  if(frame == NULL) {
    fprintf(stderr,"Could not alloc file.\n");
    exit(-1);
  }

  /* Só para ter um pixel a mais no fim do frame, para poder calcular as estatisticas em DPCM */
  frame[h*w*3/2] = 0;

  if(fread(frame, 1, h*w*3/2, input_file) != h*w*3/2) {
    fprintf(stderr,"Could not read file.\n");
    exit(-1);   
  }
  
  for(i=0; i<256; i++) {
    statistics[i] = 0;
    statistics_diff[i] = 0;
  }

  for(i=0; i<h*w; i+=w) {
    for(j=0; j<w; j++) {
      statistics[frame[i+j]]++;
      statistics_diff[abs(frame[i+j+1] - frame[i+j])]++;
    }
  }

  for(i=0; i<256; i+=16) {
    printf("|");
    for(j=0; j<16; j++) {
      printf("%5d", statistics[i+j]);
	}
    printf("|\n");
  }

  printf("\nDPCM:\n");
  
  for(i=0; i<256; i+=16) {
    printf("|");
    for(j=0; j<16; j++) {
      printf("%5d", statistics_diff[i+j]);
	}
    printf("|\n");
  }

  return 0;
}
