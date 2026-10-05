#include "PROTO.H"
#include "TYPES.H"
#include "BATTLE_PARTY.H"

#define BattleActor_SpawnObjectsForList ((s32 (*)(void*,s32))BattleActor_SpawnObjectsForList)


void BattleActor_CommitPlacement(void)
{
    u8 actor_slots[28];

    BattleParty_ListActorIds(3, (u16 *)actor_slots);
    BattleActor_SpawnObjectsForList(actor_slots, 1);
}
