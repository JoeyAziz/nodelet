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

JSValue nodelet_print(JSContext *ctx, FILE *stream, JSValueConst this_val, int argc, JSValueConst *argv) {
  const char *collect[argc];
  for (int i=0; i<argc; ++i) {
    collect[i] = JS_ToCString(ctx, argv[i]);

    if (collect[i] == NULL) {
      // free collection
      for (int j = 0; j < i; ++j) {
	JS_FreeCString(ctx, collect[j]);
      }
      return JS_EXCEPTION;
    }
  }

  for (int i=0; i<argc; ++i) {
    char *space = i==0 ? "" : " ";
    char *end   = i==argc-1 ? "\n" : "";
    fprintf(stream, "%s%s%s", space, collect[i], end);
  }
  for (int i=0; i<argc; ++i) {
    JS_FreeCString(ctx, collect[i]);
  }

  return JS_UNDEFINED;
}

JSValue nodelet_console_err(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  return nodelet_print(ctx, stderr, this_val, argc, argv);
}

JSValue nodelet_console_log(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return nodelet_print(ctx, stdout, this_val, argc, argv);
}

void js_add_console_log(JSContext *ctx) {
  JSValue global = JS_GetGlobalObject(ctx);
  JSValue console = JS_GetPropertyStr(ctx, global, "console");

  JS_SetPropertyStr(ctx, console, "log", JS_NewCFunction(ctx, nodelet_console_log, "log", 1));

  JS_FreeValue(ctx, console);
  JS_FreeValue(ctx, global);
}

void js_add_console_err(JSContext *ctx) {
  JSValue global = JS_GetGlobalObject(ctx);
  JSValue console = JS_GetPropertyStr(ctx, global, "console");

  JS_SetPropertyStr(ctx, console, "err", JS_NewCFunction(ctx, nodelet_console_err, "err", 1));

  JS_FreeValue(ctx, console);
  JS_FreeValue(ctx, global);
}

void js_attach_console(JSContext *ctx) {
  JSValue global = JS_GetGlobalObject(ctx);

  JSValue console = JS_NewObject(ctx);
  JS_SetPropertyStr(ctx, global, "console", console);
  JS_FreeValue(ctx, console);
  JS_FreeValue(ctx, global);
}

void attach_methods_to_glob_obj(JSContext *ctx) {
  js_attach_console(ctx);
  js_add_console_log(ctx);
  js_add_console_err(ctx);
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
  attach_methods_to_glob_obj(js_context);

  size_t js_module_len;
  char *js_module = load_module_file(filename, &js_module_len);

  JSValue js_eval_res = JS_Eval(js_context, js_module, js_module_len, filename, JS_EVAL_TYPE_GLOBAL);

  printf("js_eval_res = %lld", js_eval_res.tag);

  JS_FreeContext(js_context);
  JS_FreeRuntime(js_runtime);
  return 0;
}
