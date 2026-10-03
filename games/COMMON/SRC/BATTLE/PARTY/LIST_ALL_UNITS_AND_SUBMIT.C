#include "TYPES.H"
#include "BATTLE_PARTY.H"

s32 BattleActor_SpawnObjectsForList(void *, s32);

void BattleParty_ListAllUnitsAndSubmit(void)
{
    u8 local[28];
    BattleParty_ListActorIds(3, (u16 *)local);
    BattleActor_SpawnObjectsForList(local, 0);
}
