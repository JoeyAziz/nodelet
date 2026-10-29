#ifndef NODELET_EVAL_H
#define NODELET_EVAL_H

#include "quickjs.h"


int nodelet_eval(JSContext *ctx, char *filename);   // load + run sync code


#endif
