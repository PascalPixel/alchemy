#include "battle_summon.h"

extern const struct SummonDefinition Data_08084a9c[];

const struct SummonDefinition *SummonDefinition_GetNear(u32 summon_id)
{
    if (summon_id > 15)
        return NULL;
    return &Data_08084a9c[summon_id];
}
