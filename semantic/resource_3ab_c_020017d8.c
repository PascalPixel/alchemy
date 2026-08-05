#include "types.h"





extern s32 Func_02003122(s32);
extern s32 Func_02003138(s32);
extern s32 Func_02003152(s32);
extern u32 Func_02003150(void);
extern u32 Func_02003156(void);
s32 Func_020017d8(u8 *particle)
{
    s32 angle = *(s32 *)(particle + 48);
    s32 vertical = Func_02003122(angle) * 2;
    s32 facing;
    u32 wobble;

    if (vertical > 0)
        vertical = -vertical;
    *(s32 *)(particle + 8) = *(s32 *)(particle + 56) + Func_02003138(angle) * 2;
    *(s32 *)(particle + 12) = *(s32 *)(particle + 60) + vertical;

    facing = Func_02003152(angle + 0x4000) / 8;
    *(s16 *)(*(u8 **)(particle + 80) + 30) = (s16)facing;

    wobble = ((Func_02003150() << 9) >> 16);
    wobble += ((Func_02003156() << 9) >> 16);
    *(s32 *)(particle + 48) = angle + (s32)wobble + 0x400;
    return 0;
}
