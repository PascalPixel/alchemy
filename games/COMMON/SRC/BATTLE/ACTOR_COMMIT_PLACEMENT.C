#include "TYPES.H"


s32 BattleParty_ListActorIds(s32, void *);
s32 BattleActor_SpawnObjectsForList(void *, s32);

void BattleActor_CommitPlacement(void)
{
    u8 actor_slots[28];

    BattleParty_ListActorIds(3, actor_slots);
    BattleActor_SpawnObjectsForList(actor_slots, 1);
}
