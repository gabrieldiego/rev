#include "diff.h"

int create_diff(diff_t *output, image_t *input) {
  uint32_t x,y;

  output->w = input->w;
  output->h = input->h;

  output->img_size = input->img_size;

  output->buffer = (int16_t *) malloc(sizeof(int16_t)*output->img_size);
  
  if(output->buffer == NULL) {
    return -1;
  }

  /* Fazer na primeira linha separadamente */
  output->buffer[0] = input->buffer[0];

  for(x=0;x<input->w;x++) {
    output->buffer[x] = input->buffer[x+1] - input->buffer[x];
  }

  uint32_t w = output->w;
  uint32_t pitch = input->pitch;

  for(y=1;y<input->h;y++) {
    for(x=0;x<input->w;x++) {
	  uint32_t input_pos = x+y*pitch;
      output->buffer[x+y*w] = input->buffer[input_pos+1] - input->buffer[input_pos];
    }
  }

  return 0;
}

int create_img_of_abs(image_t *output, diff_t *input){
  return 0;
}
