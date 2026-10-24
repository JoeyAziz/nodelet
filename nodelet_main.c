#include <stdlib.h>
#include <stdio.h>
#include <quickjs.h>


#define fatal(fmt, ...) do {               \
    fprintf(stderr, (fmt), ##__VA_ARGS__); \
    exit(1);                               \
} while(0)

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

int main(int argc, char **argv) {
  if (argc <= 1) {
    fprintf(stderr, "USAGE: %s <FILE>", argv[0]);
    return 0;
  }
  char* filename = argv[1];

  JSRuntime *js_runtime = JS_NewRuntime();
  if (js_runtime == NULL) {
    fatal("Couldn't initialize js_runtime");
  }
  JSContext *js_context = JS_NewContext(js_runtime);
  if (js_context == NULL) {
    JS_FreeRuntime(js_runtime);
    fatal("Couldn't initialize js_context");
  }

  size_t js_module_len;
  char *js_module = load_module_file(filename, &js_module_len);

  JSValue js_eval_res = JS_Eval(js_context, js_module, js_module_len, filename, JS_EVAL_TYPE_GLOBAL);

  printf("js_eval_res = %lld", js_eval_res.tag);

  JS_FreeContext(js_context);
  JS_FreeRuntime(js_runtime);
  return 0;
}
