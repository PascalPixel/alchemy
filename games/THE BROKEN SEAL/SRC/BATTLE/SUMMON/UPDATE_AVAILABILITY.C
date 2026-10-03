#include "BATTLE_SUMMON.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_RUNTIME.H"

struct BattleSummonState *Trade_GetOfferStateFar(s32 side);

struct BattleSummonState *BattleSummon_UpdateAvailability(void)
{
    u8 totals[4];
    u16 party_members[10];
    s32 party_size;
    s32 element;
    u32 available_mask;

    party_size = BattleParty_PrepareActiveOwners(party_members);
    available_mask = 0;

    element = 0;
    do {
        totals[element] = 0;
        {
            s32 party_slot;

            for (party_slot = 0; party_slot < party_size; party_slot++) {
                struct BattleUnit *member =
                    Owner_GetStateFar(party_members[party_slot]);

                totals[element] += member->djinn_owned_counts[element];
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
            if (totals[0] >= required[0]) {
                u8 *total = totals;

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
        struct BattleSummonState *state = Trade_GetOfferStateFar(0);
        state->available_mask = available_mask;
        return state;
    }
}

void BattleSummon_ReservedNoOp(void)
{
}
