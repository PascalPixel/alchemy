#include "scene.h"

void Func_020020a8(void)
{
    struct FieldActor *leader = Object_GetById(gPartyState.current_owner);

    if (leader->y > -0x100000)
        ((u8 *)&gPartyState)[0x2c3] = 1;
    else
        ((u8 *)&gPartyState)[0x2c3] = 0;
}
