#include "types.h"

#define SceneActor_SetMode3AndFlagBit1 Func_02000b68

extern u8 *Func_0200210e(s32);
extern u8 *Func_02002116(s32);
extern void Func_0200208c(u8 *, s32);
extern void Func_020021cc(s32, s32);

void SceneActor_SetMode3AndFlagBit1(s32 no)
{
    u8 *p = Func_0200210e(no);
    u8 *flag;

    Func_0200208c(Func_02002116(no), 0);
    Func_020021cc(no, 3);
    flag = p + 85;
    *flag = 0;
    p += 35;
    {
        u8 bit = 2;

        *p = bit | *p;
    }
}
