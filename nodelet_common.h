#ifndef NODELET_COMMON_H
#define NODELET_COMMON_H

#include <stdio.h>
#include <stdlib.h>

#define fatal(fmt, ...) do {               \
    fprintf(stderr, (fmt), ##__VA_ARGS__); \
    exit(1);                               \
} while(0)


#endif
