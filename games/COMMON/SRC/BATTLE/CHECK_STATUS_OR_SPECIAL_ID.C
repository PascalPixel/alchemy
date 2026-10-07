#include "BTLSYS.H"

u32 Ability_CheckStatusOrSpecialId(s32 action_id)
{
    u8 cost;

    if (action_id == 0x7E) {
        return 1U;
    }
    cost = BtlActGet(action_id)->pp_cost;
    return (u32)((0 - cost) | cost) >> 0x1F;
}
