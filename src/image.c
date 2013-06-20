#include "image.h"

int create_img_from_config(image_t *img, config_t *config) {
  return create_img(img,config->w,config->h,config->pitch);
}

int create_img(image_t *img, uint32_t w, uint32_t h, uint32_t pitch) {
  int ret;

  ret = check_img_dimensions(w,h,pitch);
  if(ret) {
    return ret;
  }

  img->h = h;
  img->w = w;
  img->pitch = pitch;

  /* Por enquanto presupomos sempre YUV 4:2:0 */
  img->img_size = h*w*3/2;

  img->buffer = (uint8_t *)malloc(img->img_size);

  if(img->buffer == NULL) {
    return -1;
  }

  return 0;
}

int check_img_dimensions(uint32_t w, uint32_t h, uint32_t pitch) {
  /* Verificar se a imagem de entrada tem um tamanho razoável */
  if (w>10000 || h>10000) {
    return -1;
  }

  /* Verificar se o pitch não é menor que a largura */
  if (pitch < w) {
    return -2;
  }

  /* Senão está tudo ok */
  return 0;
}
