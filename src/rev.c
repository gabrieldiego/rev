#include "rev.h"

#include "config.h"
#include "image.h"
#include "diff.h"

int main(int argc, char **argv) {
  int32_t  num_pixels=0;
  config_t config;
  image_t  image;
  diff_t   diff;

  uint32_t i,j;
  uint32_t statistics[256];
  uint32_t statistics_diff[512];

  if(config_from_cmd_line(&config,argc,argv)) {
    return -1;
  }

  if(create_img_from_config(&image,&config)) {
    exit(-1);
  }

  if(fread(image.buffer, 1, image.img_size, config.input_file) != image.img_size) {
    fprintf(stderr,"Could not read file.\n");
    exit(-1);
  }

  for(i=0; i<256; i++) {
    statistics[i] = 0;
    statistics_diff[i] = 0;
  }

  create_diff(&diff,&image);

  for(i=0; i<image.h*image.w; i+=image.pitch) {
    for(j=0; j<image.w; j++) {
	  fwrite(image.buffer+i+j,1,1,config.output_file);
      num_pixels++;
      statistics[image.buffer[i+j]]++;
      statistics_diff[abs(diff.buffer[i+j])]++;
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
