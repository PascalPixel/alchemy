#ifndef ALCHEMY_OBJECT_COMMANDS_H
#define ALCHEMY_OBJECT_COMMANDS_H

#include "types.h"

/*
 * These routines accept several established views of the same 0x70-byte
 * object record. Keep the public contract opaque and select the view inside
 * each implementation.
 */
void Func_0800c300(void *object, s32 argument);
s32 Func_0800d14c(void *object, s32 x, s32 y, s32 z);

#endif
