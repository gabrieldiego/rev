#include "rev.h"
#include "config.h"

int main(int argc, char **argv) {
  int32_t num_pixels=0;
  config_t config;

  uint32_t i,j;
  uint32_t statistics[256];
  uint32_t statistics_diff[512];
  uint8_t *frame;
  
  if(config_from_cmd_line(&config,argc,argv)) {
    return -1;
  }
  
  frame = malloc(config.img_size+1);
  
  if(frame == NULL) {
    fprintf(stderr,"Não pode alocar frame buffer.\n");
    exit(-1);
  }

  /* Só para ter um pixel a mais no fim do frame, para poder calcular as estatisticas em DPCM */
  frame[config.img_size] = 0;

  if(fread(frame, 1, config.img_size, config.input_file) != config.img_size) {
    fprintf(stderr,"Could not read file.\n");
    exit(-1);   
  }
  
  for(i=0; i<256; i++) {
    statistics[i] = 0;
    statistics_diff[i] = 0;
  }

  for(i=0; i<config.h*config.w; i+=config.pitch) {
    for(j=0; j<config.w; j++) {
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
