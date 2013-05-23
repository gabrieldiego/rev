#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "rev.h"
#include "config.h"

typedef struct {
  uint32_t h;
  uint32_t w;
  uint32_t pitch;

  uint32_t img_size;
  uint8_t *buffer;
} image_t;

int create_img_from_config(image_t *img, config_t *config);

int create_img(image_t *img, uint32_t w, uint32_t h, uint32_t pitch);

int check_img_dimensions(uint32_t w, uint32_t h, uint32_t pitch);

#endif /* __IMAGE_H_ */
