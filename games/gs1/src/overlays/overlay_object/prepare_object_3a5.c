#include "types.h"

#define NULL ((void *)0)

void *Func_02001d56(s32, s32, s32, s32);
void Func_02001db4(void *, s32);
void Func_02001dcc(void *, s32);
void Func_02001e5c(void *, s32);

void *OverlayObject_PrepareObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj = Func_02001d56(arg3, arg0, arg1, arg2);

    if (obj != NULL) {
        u8 *rec = *(u8 **)(obj + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = rec[9];
        mask = -mask;
        mask &= flags;
        rec[9] = mask;
        obj[0x55] = 0;
        obj[0x59] = 8;
        Func_02001db4(obj, 0);
        Func_02001e5c(obj, 14);
        Func_02001dcc(obj, 1);
        return obj;
    }
    return NULL;
}
