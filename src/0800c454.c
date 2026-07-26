#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_0800c2d8(void *, s32);

void Func_0800c454(void *arg0, s32 arg1) {
    Func_0800c2d8(arg0, 0x08013608);
    M2C_FIELD(arg0, s32 *, 0x68) = arg1;
}
