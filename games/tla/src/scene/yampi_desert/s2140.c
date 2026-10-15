#include "scene.h"

void Func_02002140(void)
{
    struct FieldActor *leader = Object_GetById(gPartyState.currentOwner);
    u32 x = leader->x >> 20;
    u32 z = leader->z >> 20;

    if (x > 12 && z > 10 && x <= 15 && z <= 13)
        GameFlag_SetBit(263);
    else
        GameFlag_ClearBit(263);
}
