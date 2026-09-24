#include "TYPES.H"


s32 BattleParty_ListActorIds(s32, void *);
s32 Func_080b7b6c(void *, s32);

void BattleActor_CommitPlacement(void)
{
    u8 actor_slots[28];

    BattleParty_ListActorIds(3, actor_slots);
    Func_080b7b6c(actor_slots, 1);
}
