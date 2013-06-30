#include "config.h"
#include "image.h"

int config_from_cmd_line(config_t *config, int argc, char **argv) {
  if(argc < 5) {
    fprintf(stderr,"Poucos argumentos.\n");
    fprintf(stderr,"Uso: %s <yuv entrada> <largura> <altura> <yuv saida>.\n",
      argv[0]);
	return -1;
  }

  config->input_file = fopen(argv[1],"rb");

  if(config->input_file == NULL) {
    fprintf(stderr,"Não foi possível abrir arquivo %s para leitura.\n",argv[1]);
	return -1;
  }

  if(init_img_dimensions(config,atoi(argv[3]),atoi(argv[2]))) {
    fprintf(stderr,"Dimensões inválidas.\n");
	return -1;
  }

  config->output_file = fopen(argv[4],"wb");

  if(config->output_file == NULL) {
    fprintf(stderr,"Não foi possível abrir arquivo %s para escrita.\n",argv[4]);
	return -1;
  }

  return 0;
}

int init_img_dimensions(config_t *config, uint32_t h, uint32_t w) {

  if(check_img_dimensions(w, h, w)) {
    return -2;
  }

  if(config == NULL) {
    return -1;
  }

  config->h = h;
  config->w = w;
  config->pitch = w;
  
  /* Por enquanto presupomos sempre YUV 4:2:0 */
  config->img_size = h*w*3/2;

  return 0;
}
