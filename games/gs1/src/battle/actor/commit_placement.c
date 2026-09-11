#include "types.h"
#include "scene.h"
#include "abi/battle/actor/commit_placement.h"

void BattleActor_CommitPlacement(void)
{
    u8 actor_slots[28];

    Actor_Apply(3, actor_slots);
    Actor_Apply2(actor_slots, 1);
}
