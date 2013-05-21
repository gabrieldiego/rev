#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "rev.h"
#include <stdint.h>

typedef struct {
  FILE *input_file;
  FILE *output_file;
  
  uint32_t w;
  uint32_t h;

} config_t;

#endif /* __CONFIG_H__ */
