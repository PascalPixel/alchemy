#include "types.h"

#define OverlayObject_ApplyZero Func_02000690

extern void Func_020054da(void *, s32);

s32 OverlayObject_ApplyZero(void *obj)
{
    Func_020054da(obj, 0);
    return 0;
}
