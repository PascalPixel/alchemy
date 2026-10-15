#include "trial_road.h"

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
