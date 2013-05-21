#include "rev.h"
#include "config.h"

int main(int argc, char **argv) {
  int32_t w,h;
  int32_t num_pixels=0;
  config_t config;

  if(argc < 5) {
    fprintf(stderr,"Few arguments.\n");
	exit(-1);
  }

  config.input_file = fopen(argv[1],"rb");

  if(config.input_file == NULL) {
    fprintf(stderr,"Could not open file %s for reading.\n",argv[1]);
    exit(-1);
  }

  w = atoi(argv[2]);
  h = atoi(argv[3]);

  if(h <= 0 || w <= 0) {
    fprintf(stderr,"Invalid dimensions.\n");
    exit(-1);
  }

  config.output_file = fopen(argv[4],"wb");

  if(config.output_file == NULL) {
    fprintf(stderr,"Could not open file %s for writting.\n",argv[4]);
    exit(-1);
  }

  uint32_t i,j;
  uint32_t statistics[256];
  uint32_t statistics_diff[512];
  uint8_t *frame;
  
  frame = malloc(h*w*3/2+1);
  
  if(frame == NULL) {
    fprintf(stderr,"Could not alloc file.\n");
    exit(-1);
  }

  /* Só para ter um pixel a mais no fim do frame, para poder calcular as estatisticas em DPCM */
  frame[h*w*3/2] = 0;

  if(fread(frame, 1, h*w*3/2, config.input_file) != h*w*3/2) {
    fprintf(stderr,"Could not read file.\n");
    exit(-1);   
  }
  
  for(i=0; i<256; i++) {
    statistics[i] = 0;
    statistics_diff[i] = 0;
  }

  for(i=0; i<h*w; i+=w) {
    for(j=0; j<w; j++) {
	  fwrite(frame+i+j,1,1,config.output_file);
      num_pixels++;
      statistics[frame[i+j]]++;
      statistics_diff[abs(frame[i+j+1] - frame[i+j])]++;
    }
  }

  fclose(config.output_file);

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

  printf("\nDPCM nr bits:\n");
  
  for(i=0; i<256; i+=16) {
    printf("|");
    for(j=0; j<16; j++) {
      printf("%6.2f", log((1.0*num_pixels)/statistics_diff[i+j])/log(2));
	}
    printf("|\n");
  }

  return 0;
}
