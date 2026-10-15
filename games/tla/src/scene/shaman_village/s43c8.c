#include "shaman_village.h"

void Func_020043c8(void)
{
    s32 variant = Object_GetById(gPartyState.current_owner)->sprite->priority;

    ObjectMotion_SetActionVariant(21, variant);
    ObjectMotion_SetActionVariant(14, variant);
}
