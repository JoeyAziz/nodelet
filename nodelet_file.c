#include "nodelet_file.h"

char *load_module_file(char *filename, size_t *len) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    fatal("Couldn't open JS file");
  }

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buffer = malloc(size+1);
  if (buffer == NULL) {
    fclose(file);
    fatal("OOM!");
  }

  size_t read = fread(buffer, 1, size, file);
  if (read != (size_t)size) {
    fclose(file);
    fatal("Error in reading file -- %s", filename);
  }

  *len = (size_t)size;
  return buffer; 
}
