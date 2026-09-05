#include "types.h"

#define BattleActor_CommitPlacement Func_080b6c90

s32 Func_080b6c08(s32, void *);
s32 Func_080b7b6c(void *, s32);

void BattleActor_CommitPlacement(void)
{
    u8 actor_slots[28];

    Func_080b6c08(3, actor_slots);
    Func_080b7b6c(actor_slots, 1);
}
