#include "types.h"

#define NULL ((void *)0)

void *Func_0200127e(s32, s32, s32, s32);
void Func_020012bc(void *, s32);
void Func_020012d4(void *, s32);
void Func_0200137c(void *, s32);

void *OverlayObject_CreateAndInitialize(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj = Func_0200127e(arg3, arg0, arg1, arg2);

    if (obj != NULL) {
        u8 *sprite = *(u8 **)(obj + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = sprite[9];
        mask = -mask;
        mask &= flags;
        sprite[9] = mask;
        obj[0x55] = 0;
        obj[0x59] = 8;
        Func_020012bc(obj, 0);
        Func_0200137c(obj, 14);
        Func_020012d4(obj, 1);
        return obj;
    }
    return NULL;
}
