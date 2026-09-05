#include "types.h"

#define OverlayObject_ActivateSlotWithMode3 Func_02003380

extern u8 *Func_02007692(s32);
extern void Func_02007778(s32, s32);
extern void Func_02007640(u8 *, s32);

void OverlayObject_ActivateSlotWithMode3(s32 a)
{
    u8 *p = Func_02007692(a);

    if (p != 0) {
        Func_02007778(a, 3);
        Func_02007640(p, 0);
        p[89] = 0;
        {
            s32 c;
            c = 2 | p[35];
            p[35] = c;
        }
    }
}
