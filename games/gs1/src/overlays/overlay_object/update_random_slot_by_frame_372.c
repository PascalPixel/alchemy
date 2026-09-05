#include "types.h"

#define OverlayObject_UpdateRandomSlotByFrame Func_02004304

extern s32 Func_02008950(s32, s32);
extern void Func_020089f0(s32, s32);
extern s32 Func_02008966(s32, s32);
extern void Func_02008758(s32);

void OverlayObject_UpdateRandomSlotByFrame(s32 obj)
{
    volatile s32 *fc = (volatile s32 *)0x03001e40;
    s32 t;
    s32 n;

    if ((*fc & 1) != 0) {
        t = (s32)((u32)*fc >> 1);
        Func_020089f0(obj, Func_02008950(t, 6));
    }
    n = (*(s32 *)0x0200d7fc << 3) + 16;
    if (Func_02008966(*fc, n) == 0) {
        Func_02008758(obj);
    }
}
