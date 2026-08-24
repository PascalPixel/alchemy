#include "battle_summon.h"
#include "battle_party.h"
#include "owner_state.h"

struct PartyDjinnTotals {
    u8 by_element[4];
};

struct BattleSummonState *Func_08077000(s32 side);

struct BattleSummonState *BattleSummon_UpdateAvailability(void)
{
    struct PartyDjinnTotals totals;
    u16 party_members[10];
    s32 party_size;
    s32 element;
    u32 available_mask;

    party_size = BattleParty_ListActiveMembers(party_members);
    available_mask = 0;

    element = 0;
    do {
        totals.by_element[element] = 0;
        {
            s32 party_slot;

            for (party_slot = 0; party_slot < party_size; party_slot++) {
                struct OwnerValueState *member =
                    OwnerState_GetFar(party_members[party_slot]);

                totals.by_element[element] += member->values[element];
            }
        }
        element++;
    } while (element <= 3);

    element = 0;
    do {
        const struct SummonDefinition *summon =
            SummonDefinition_Get(element);
        s32 elements_met;

        if (summon != 0) {
            const u8 *required = summon->djinn_required;

            elements_met = 0;
            if (totals.by_element[0] >= required[0]) {
                u8 *total = totals.by_element;

                do {
                    elements_met++;
                    if (elements_met > 3)
                        break;
                    total++;
                    required++;
                } while (*total >= *required);
            }

            if (elements_met == 4)
                available_mask |= 1u << element;
        }
        element++;
    } while (element <= 31);

    {
        struct BattleSummonState *state = Func_08077000(0);
        state->available_mask = available_mask;
        return state;
    }
}

void BattleSummon_ReservedNoOp5E10(void)
{
}
