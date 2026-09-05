#include "types.h"

#define OverlayObject_UpdateEveryFourFrames Func_020005ac

extern void Func_020054fa(void *, s32);
extern void Func_02005502(void *, s32);
extern void Func_020055d4(s32);

s32 OverlayObject_UpdateEveryFourFrames(void *obj)
{
    if ((*(u32 *)0x03001e40 & 3) == 0)
        Func_020054fa(obj, 7);
    else
        Func_02005502(obj, 0);

    if ((*(u32 *)0x03001e40 & 7) == 0)
        Func_020055d4(138);
    return 0;
}
