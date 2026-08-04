#include "layout_guard.h"
#include "types.h"

typedef struct BattleUnit_080b920c {
    u8 padding000[0x40];
    u16 weight;
    u8 padding042;
    u8 action_count;
    u8 padding044[0xf4];
    u8 delay_penalty;
    u8 status_139;
    u8 auto_battle;
    u8 stunned;
    u8 downed;
} BattleUnit_080b920c;

typedef struct BattleDecision_080b920c {
    s16 owner;
    u8 padding02[2];
    u16 weight;
    s16 state;
    u16 action;
    s16 target;
    u8 padding0c[4];
} BattleDecision_080b920c;

LAYOUT_OFFSET_GUARD(
    BattleUnit080b920c_Weight,
    BattleUnit_080b920c,
    weight,
    0x40);
LAYOUT_OFFSET_GUARD(
    BattleUnit080b920c_ActionCount,
    BattleUnit_080b920c,
    action_count,
    0x43);
LAYOUT_OFFSET_GUARD(
    BattleUnit080b920c_DelayPenalty,
    BattleUnit_080b920c,
    delay_penalty,
    0x138);
LAYOUT_OFFSET_GUARD(
    BattleUnit080b920c_Downed,
    BattleUnit_080b920c,
    downed,
    0x13c);
LAYOUT_SIZE_GUARD(
    BattleDecision080b920c_Size,
    BattleDecision_080b920c,
    0x10);
LAYOUT_OFFSET_GUARD(
    BattleDecision080b920c_Weight,
    BattleDecision_080b920c,
    weight,
    4);
LAYOUT_OFFSET_GUARD(
    BattleDecision080b920c_State,
    BattleDecision_080b920c,
    state,
    6);
LAYOUT_OFFSET_GUARD(
    BattleDecision080b920c_Target,
    BattleDecision_080b920c,
    target,
    0x0a);

void Func_08002df0(void *allocation);
void *Func_08004970(s32 pool);
s32 Func_080151c0(
    BattleDecision_080b920c *output, u16 *owners, s32 owner_count);
BattleUnit_080b920c *Func_08077008(s32 owner);
s32 Func_080b6b40(s32 groups, u16 *owners);

static s32 NeedsAutomaticDecision_080b920c(
    const BattleUnit_080b920c *unit)
{
    return unit->downed != 0 ||
        unit->status_139 != 0 ||
        unit->auto_battle != 0 ||
        unit->stunned != 0;
}

/*
 * Seed party decisions that do not require the command UI, then ask the UI
 * to append decisions for every remaining owner/action slot. The return value
 * is the combined decision count, or -1 when manual selection is cancelled.
 */
s32 Func_080b920c(BattleDecision_080b920c *output)
{
    u16 *manual_owners = Func_08004970(0x11);
    u16 *party_owners = Func_08004970(9);
    s32 party_count = Func_080b6b40(1, party_owners);
    s32 automatic_count = 0;
    s32 manual_count = 0;
    s32 owner_number;
    s32 result;

    for (owner_number = 0;
         owner_number < party_count;
         owner_number++) {
        u16 owner = party_owners[owner_number];
        BattleUnit_080b920c *unit = Func_08077008(owner);
        s32 action_number;

        for (action_number = 0;
             action_number < unit->action_count;
             action_number++) {
            if (NeedsAutomaticDecision_080b920c(unit)) {
                BattleDecision_080b920c *decision =
                    &output[automatic_count++];

                decision->owner = owner;
                decision->weight = unit->weight;
                decision->state = 8;
                decision->action = 0;
                decision->target = 0x180;
            } else {
                manual_owners[manual_count++] = owner;
            }
        }
    }

    result = Func_080151c0(
        &output[automatic_count], manual_owners, manual_count);
    if (result >= 0)
        result += automatic_count;

    Func_08002df0(party_owners);
    Func_08002df0(manual_owners);
    return result;
}
