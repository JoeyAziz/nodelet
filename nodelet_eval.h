#ifndef NODELET_EVAL_H
#define NODELET_EVAL_H

#include "quickjs.h"


int nodelet_eval(JSContext *ctx, char *filename);   // load + run sync code
int nodelet_run_micro_queue(JSRuntime *rt);         // drain pending jobs


#endif
