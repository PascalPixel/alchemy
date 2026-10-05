#include "PROTO.H"
#include "TYPES.H"
#include "BATTLE_PARTY.H"

#define BattleActor_SpawnObjectsForList ((s32 (*)(void*,s32))BattleActor_SpawnObjectsForList)


void BattleParty_ListAllUnitsAndSubmit(void)
{
    u8 local[28];
    BattleParty_ListActorIds(3, (u16 *)local);
    BattleActor_SpawnObjectsForList(local, 0);
}
