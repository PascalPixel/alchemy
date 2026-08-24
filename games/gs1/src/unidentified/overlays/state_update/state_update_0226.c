#include "types.h"

extern u8 *Func_0200210e(s32);
extern u8 *Func_02002116(s32);
extern void Func_0200208c(u8 *, s32);
extern void Func_020021cc(s32, s32);

void Func_02000b68(s32 actor)
{
    u8 *record = Func_0200210e(actor);
    u8 *flags;

    Func_0200208c(Func_02002116(actor), 0);
    Func_020021cc(actor, 3);
    flags = record + 85;
    *flags = 0;
    record += 35;
    {
        u8 bit = 2;

        *record = bit | *record;
    }
}
