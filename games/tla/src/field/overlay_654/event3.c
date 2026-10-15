#include "shrine_of_the_sea_god.h"

extern s32 Data_02004a18[];

void Func_0200142c(struct ShrineOfTheSeaGodObject *object)
{
    struct ShrineOfTheSeaGodObject *leader = Object_GetById(gPartyState.current_owner);
    s32 *offset = Data_02004a18;

    object->x = leader->x + offset[0];
    object->y = leader->y + offset[1];
    object->z = leader->z + offset[2];
}

void Func_02001464(void)
{
    Func_0200142c(Object_GetById(14));
}
