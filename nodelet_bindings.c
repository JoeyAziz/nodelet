#include "nodelet_bindings.h"

static JSValue nodelet_print(JSContext *ctx, FILE *stream, JSValueConst this_val, int argc, JSValueConst *argv) {
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

static JSValue nodelet_console_err(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  return nodelet_print(ctx, stderr, this_val, argc, argv);
}

static JSValue nodelet_console_log(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return nodelet_print(ctx, stdout, this_val, argc, argv);
}

static void js_add_console_log(JSContext *ctx) {
  JSValue global = JS_GetGlobalObject(ctx);
  JSValue console = JS_GetPropertyStr(ctx, global, "console");

  JS_SetPropertyStr(ctx, console, "log", JS_NewCFunction(ctx, nodelet_console_log, "log", 1));

  JS_FreeValue(ctx, console);
  JS_FreeValue(ctx, global);
}

static void js_add_console_err(JSContext *ctx) {
  JSValue global = JS_GetGlobalObject(ctx);
  JSValue console = JS_GetPropertyStr(ctx, global, "console");

  JS_SetPropertyStr(ctx, console, "err", JS_NewCFunction(ctx, nodelet_console_err, "err", 1));

  JS_FreeValue(ctx, console);
  JS_FreeValue(ctx, global);
}

static void js_attach_console(JSContext *ctx) {
  JSValue global = JS_GetGlobalObject(ctx);
  JSValue console = JS_NewObject(ctx);
  JS_SetPropertyStr(ctx, global, "console", console);
  JS_FreeValue(ctx, global);
}

void nodelet_bindings_attach(JSContext *ctx) {
  js_attach_console(ctx);
  js_add_console_log(ctx);
  js_add_console_err(ctx);
}
