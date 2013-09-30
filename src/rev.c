#include "rev.h"

#include "config.h"
#include "image.h"
#include "diff.h"
#include "huffman.h"
#include "bitbang.h"

int main(int argc, char **argv) {
  int32_t  num_pixels=0;
  config_t config;
  image_t  image;
  diff_t   diff;
  huffman_tree_t ht;

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

  create_huffman_tree(&ht, image.buffer, image.img_size);

  printf("height %d width %d pitch %d size %d\n",
    image.h,image.w,image.pitch,image.img_size);

#if 0
  for(i=0; i<image.h*image.pitch; i+=image.pitch) {
    for(j=0; j<image.w; j++) {
      fwrite(image.buffer+i+j,1,1,config.output_file);
      num_pixels++;
      statistics_diff[abs(diff.buffer[i+j])]++;
    }
  }

  fclose(config.output_file);
#endif

  for(i=0; i<256; i+=16) {
    printf("|");
    for(j=0; j<16; j++) {
      printf("%5d", ht.list[i+j].leaf->occurrence);
    }
    printf("|\n");
  }
  
  printf("\n");

  for(i=0; i<256; i+=16) {
    printf("|");
    for(j=0; j<16; j++) {
      int len = strlen(ht.list[i+j].leaf->bitstring);
      if(len<=9)
        if(len)
          printf("%9.9s", ht.list[i+j].leaf->bitstring);
        else
          printf("     nula");
      else
        printf("    longa");
    }
    printf("|\n");
  }

  bitwrite_t *bw = malloc(sizeof(bitwrite_t));

  if(bw == NULL) {
    exit(-1);
  }

  if(create_bitwrite(bw,config.output_file)) {
    exit(-1);
  }

  write_huffman_tree_to_file(ht.root,bw);

  for(i=0; i<image.img_size; i++) {
    write_bitstring(bw,ht.list[image.buffer[i]].leaf->bitstring);
  }

  i = close_bitwrite(bw);
  if(i) {
    exit(i);
  }

  bitread_t br;
  fclose(config.output_file);
  config.output_file = fopen(config.output_file_name,"rb");
  
  create_bitread(&br,config.output_file);

  FILE *rebuilt_file;

  rebuilt_file = fopen("test.yuv","wb");
  if (rebuilt_file == NULL) {
    fprintf(stderr,"Não foi possível abrir arquivo test.yuv");
    exit(-1);
  }

  for(i=0; i<image.img_size; i++) {
    ;
  }


#if 0
  printf("\nOccurences in order:\n");

  huffman_list_t *list = ht.smallest;

  for(i=0; i<256; i+=16) {
    printf("|");
    for(j=0; j<16; j++) {
      printf("%3d:%-5d",list->leaf->symbol,list->leaf->occurrence);
      list = list->bigger;
    }
    printf("|\n");
  }
#endif

#if 0
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

#endif
  return 0;
}
