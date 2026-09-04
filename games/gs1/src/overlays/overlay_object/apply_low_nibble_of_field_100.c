#include "types.h"

#define OverlayObject_ApplyLowNibbleOfField100 Func_02000594

extern void Func_020054de(void *, s32);

s32 OverlayObject_ApplyLowNibbleOfField100(void *obj)
{
    Func_020054de(obj, *(u16 *)((u8 *)obj + 100) & 15);
    return 0;
}
