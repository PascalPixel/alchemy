#include "types.h"

extern s32 Func_03000380(s32 value, s32 divisor);

/* Integrate one drifting particle and feed its age into the owner record. */
void Func_0200185c(u8 *particle)
{
    u8 *owner;

    *(s32 *)(particle + 8) += *(s32 *)(particle + 68);
    *(s32 *)(particle + 12) += *(s32 *)(particle + 72);
    *(s32 *)(particle + 16) += *(s32 *)(particle + 76);

    *(s32 *)(particle + 68) -=
        Func_03000380(*(s32 *)(particle + 68), 18);
    *(s32 *)(particle + 76) -= *(s32 *)(particle + 76) / 16;

    *(s32 *)(particle + 24) += *(s32 *)(particle + 48);
    *(s32 *)(particle + 28) += *(s32 *)(particle + 52);

    owner = *(u8 **)(particle + 80);
    *(u16 *)(owner + 30) += *(u16 *)(particle + 100);
}
