#include "BATTLE_SUMMON.H"

extern const struct SummonDefinition Summon_DefinitionTable[];

const struct SummonDefinition *SummonDefinition_GetNear(u32 summon_id)
{
    if (summon_id > 28)
        return NULL;
    return &Summon_DefinitionTable[summon_id];
}
