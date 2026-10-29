#include "nodelet_eval.h"
#include "nodelet_file.h"

static void report_exception(JSContext *ctx) {
  JSValue exc = JS_GetException(ctx);
  const char *js_msg = JS_ToCString(ctx, exc);
  const char* msg = js_msg == NULL ? "Unknown" : js_msg;
  fprintf(stderr, "Exception:\n\t%s\n", msg);
  JS_FreeCString(ctx, js_msg);
  JS_FreeValue(ctx, exc); 
}

int nodelet_eval(JSContext *ctx, char *filename) {
  size_t js_module_len;
  char *js_module = load_module_file(filename, &js_module_len);

  int res = 0;
  JSValue js_eval_res = JS_Eval(ctx, js_module, js_module_len, filename, JS_EVAL_TYPE_GLOBAL);
  if (JS_IsException(js_eval_res)) {
    report_exception(ctx);
    res = 1;
  }

  JS_FreeValue(ctx, js_eval_res);
  return res;
}
