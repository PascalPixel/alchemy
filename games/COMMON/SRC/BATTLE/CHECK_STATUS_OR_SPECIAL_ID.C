#include "BATTLE_RUNTIME.H"

/* The special action is accepted without a PP cost; other actions need one. */
u32 Ability_CheckStatusOrSpecialId(s32 action_id)
{
    u8 cost;

    if (action_id == 0x7E) {
        return 1U;
    }
    cost = BattleAction_Get(action_id)->pp_cost;
    return (u32)((0 - cost) | cost) >> 0x1F;
}
