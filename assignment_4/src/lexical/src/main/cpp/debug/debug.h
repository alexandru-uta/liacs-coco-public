#ifndef _DEBUG_H
#define _DEBUG_H
#include <stdarg.h>
#include <stdio.h>

#if defined(__cplusplus)
extern "C" {
#endif

void pmesg(int level, const char* format, ...);

#if defined(__cplusplus)
}
#endif

#endif
