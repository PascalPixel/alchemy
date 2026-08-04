#include "types.h"

extern s32 Func_08000118(s32);
extern s32 Func_08000120(s32);
extern u32 Func_080000f8(void);

s32 Func_020017d8(u8 *particle)
{
    s32 angle = *(s32 *)(particle + 48);
    s32 vertical = Func_08000118(angle) * 2;
    s32 facing;
    u32 wobble;

    if (vertical > 0)
        vertical = -vertical;
    *(s32 *)(particle + 8) = *(s32 *)(particle + 56) + Func_08000120(angle) * 2;
    *(s32 *)(particle + 12) = *(s32 *)(particle + 60) + vertical;

    facing = Func_08000120(angle + 0x4000) / 8;
    *(s16 *)(*(u8 **)(particle + 80) + 30) = (s16)facing;

    wobble = ((Func_080000f8() << 9) >> 16);
    wobble += ((Func_080000f8() << 9) >> 16);
    *(s32 *)(particle + 48) = angle + (s32)wobble + 0x400;
    return 0;
}
