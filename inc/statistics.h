#ifndef __STATISTICS_H_
#define __STATISTICS_H_

#include "rev.h"
#include "statistics.h"

typedef struct {
  int32_t initial_symbol;
  int32_t final_symbol;

  uint32_t len;

  uint32_t *buffer;
  /* The same buffer as above, but with a offset of the */
  uint32_t *ocurrence;
} stats_vector_t;

int create_stats_vector(diff_t *output, image_t *input);

int create_img_of_abs(image_t *output, diff_t *input);

#endif /* __STATISTICS_H_ */
