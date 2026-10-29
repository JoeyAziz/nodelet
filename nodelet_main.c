#include <stdio.h>
#include "nodelet_common.h"
#include "quickjs.h"
#include "nodelet_bindings.h"
#include "nodelet_eval.h"


int main(int argc, char **argv) {
  if (argc != 2) {
    fatal("USAGE: %s <FILE>", argv[0]);
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
  nodelet_bindings_attach(js_context);

  int eval_res = nodelet_eval(js_context, filename);
  
  JS_FreeContext(js_context);
  JS_FreeRuntime(js_runtime);  
  return eval_res;
}
