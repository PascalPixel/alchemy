#include "TYPES.H"

s32 BattleParty_ListActorIds(s32, void *);
s32 BattleActor_SpawnObjectsForList(void *, s32);

void BattleParty_ListAllUnitsAndSubmit(void)
{
    u8 local[28];
    BattleParty_ListActorIds(3, local);
    BattleActor_SpawnObjectsForList(local, 0);
}
