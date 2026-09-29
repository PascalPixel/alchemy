#include "MORI.H"

s32 OverlayObject_ApplyField100(s32 a)
{
    Object_SetPalette(a, *(s16 *)(a + 100));
    return 0;
}

s32 OverlayObject_ApplyZero(s32 a)
{
    Actor_SetSpriteFlags(a, 0);
    return 0;
}
