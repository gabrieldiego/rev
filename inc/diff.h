#ifndef __DIFF_H_
#define __DIFF_H_

#include "rev.h"
#include "image.h"
#include "diff.h"

typedef struct {
  uint32_t h;
  uint32_t w;

  uint32_t img_size;
  int16_t *buffer;
} diff_t;

int create_diff(diff_t *output, image_t *input);

int create_img_of_abs(image_t *output, diff_t *input);

#endif /* __DIFF_H_ */
