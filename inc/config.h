#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "rev.h"

typedef struct {
  FILE *input_file;
  FILE *output_file;

  uint32_t h;
  uint32_t w;
  uint32_t pitch;

  uint32_t img_size;
} config_t;

int config_from_cmd_line(config_t *config, int argc, char **argv);

int init_img_dimensions(config_t *config, uint32_t h, uint32_t w);

#endif /* __CONFIG_H__ */
