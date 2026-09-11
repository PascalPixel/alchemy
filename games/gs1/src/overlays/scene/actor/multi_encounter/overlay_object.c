#include "types.h"

#define OverlayObject_DecayFields24And28 Func_020008c0
#define OverlayObject_ActivateSlotWithMode3 Func_02003380
u8 *Func_02007692(s32);
void Func_02007778(s32, s32);
void Func_02007640(u8 *, s32);

void OverlayObject_DecayFields24And28(u8 *o)
{
    if (*(s32 *)(o + 24) > 0x10000) {
        *(s32 *)(o + 24) += 0xFFFFF800;
        *(s32 *)(o + 28) += 0xFFFFF800;
    }
}

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
