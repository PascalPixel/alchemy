#include "types.h"

extern s32 Func_08000118();

/* Advance a spawned companion for 32 frames, following its owning actor. */
s32 Func_02001c20(u8 *object)
{
    u8 *owner = *(u8 **)(object + 104);
    s32 age = (short)(*(unsigned short *)(object + 100) + 1);
    *(unsigned short *)(object + 100) = (unsigned short)age;
    if (age > 31)
        return 0;
    *(s32 *)(object + 24) = Func_08000118(age << 10);
    *(s32 *)(object + 28) = *(s32 *)(object + 24);
    *(s32 *)(object + 8) = *(s32 *)(owner + 8);
    *(s32 *)(object + 12) += 0x10000;
    *(s32 *)(object + 16) = *(s32 *)(owner + 16);
    return 1;
}
