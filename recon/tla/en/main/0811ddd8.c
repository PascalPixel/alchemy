/*
 * Draft of Func_0811ddd8: turns a target list into an action plan. 10
 * instructions differ, five swapped pairs (the argument copy and the 0xfff
 * mask, the side load, the counter copy, a constant); the logic, the frame
 * and the spilled counter match.
 */
#include "types.h"
#include "owner_state.h"

struct ActionTargets {
    u8 side;
    s8 count;
    u8 unknown_02;
    u8 ids[14];
    u8 state_a[14];
    u8 state_b[14];
    u8 state_c[14];
    u8 state_d[14];
    u16 value_4a;
    s32 message;
    s32 value_50;
    s32 value_54;
    u32 cue;
    s32 unknown_5c;
    s32 extra;
    s32 unknown_64;
};

struct ActionPlan {
    s32 cue;
    s32 unknown_04;
    s32 side;
    s32 first;
    s32 one;
    s32 count;
    s32 kind;
    s32 zero;
    s32 unknown_20;
    u16 ids[14];
};

u32 BattleParty_IsUnitListed(u32 id);

void Func_0811ddd8(struct ActionTargets *targets, struct ActionPlan *plan)
{
    s32 count = 0;
    s32 i;

    plan->zero = 0;
    plan->cue = targets->cue & 0xfff;
    plan->kind = (targets->cue & 0x3000) >> 12;
    plan->side = targets->side;
    for (i = 0; i < targets->count; i++) {
        if (BattleParty_IsUnitListed(targets->ids[i])) {
            struct BtlUnit *unit = UnitGet(targets->ids[i]);

            if (unit->hp != 0 || (targets->cue & 0x10000)) {
                plan->ids[count] = targets->ids[i];
                count++;
            }
        }
    }
    if (count == 0) {
        plan->ids[0] = targets->ids[0];
        count = 1;
    }
    plan->first = targets->ids[0];
    plan->count = count;
    plan->one = 1;
}
