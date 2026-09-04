#include "types.h"

#define OverlayObject_CreateAndInitialize Func_02000048
#define NULL ((void *)0)

void *Func_02005cc6(s32, s32, s32, s32);
void Func_02005d2c(void *, s32);
void Func_02005d44(void *, s32);
void Func_02005e74(void *, s32);

void *OverlayObject_CreateAndInitialize(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret = Func_02005cc6(kind, x, y, z);

    if (ret != NULL) {
        u8 *obj = *(u8 **)(ret + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = obj[9];
        mask = -mask;
        mask &= flags;
        obj[9] = mask;
        ret[0x55] = 0;
        ret[0x59] = 8;
        Func_02005d2c(ret, 0);
        Func_02005e74(ret, 14);
        Func_02005d44(ret, 1);
        return ret;
    }
    return NULL;
}
