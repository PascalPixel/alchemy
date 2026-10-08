#include "local.h"

void Func_020003b0(void)
{
    Object_GetById(gPartyState.current_owner)->terrain_id = 2;
}

void Func_020003cc(void)
{
    Object_GetById(gPartyState.current_owner)->terrain_id = 0;
}
