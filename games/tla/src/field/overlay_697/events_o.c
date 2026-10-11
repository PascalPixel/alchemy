#include "ov697.h"

static __inline__ void Map_CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005818(a, b, c, d, e, f);
}

void Func_020011c0(void)
{
    Object_GetById(gPartyState.current_owner)->terrain_id = 2;
}

void Func_020011dc(void)
{
    Object_GetById(gPartyState.current_owner)->terrain_id = 0;
}

void Func_020011f8(void)
{
    Map_CopyCells(12, 21, 1, 1, 19, 21);
    GameFlag_SetBit(0x374);
}
