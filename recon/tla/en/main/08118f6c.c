/* Draft, differ 2 of 106 (2026-10-15), ported from The Broken Seal's BattleSummon_UpdateAvailability:
 * this game keeps the high half of the offer state's mask, tests 16 summons and writes the mask
 * back. The native reloads the party list's address from its stack slot (ldr r6, [sp, #4]) between
 * the two halves of the 0x118 offset (movs r1, #140; lsls r1, r1, #1); here the offset is finished
 * first. Walking the list by pointer, a barrier after clearing the total, and three minutes of
 * alchemy permute measured 2 to 4. The listing ends with a two-byte empty function (bx lr) at
 * 0x08119050 that adopts with it. */
#include "types.h"
#include "battle_summon.h"
#include "battle_party.h"
#include "battle_unit.h"

struct BattleSummonState *Trade_GetOfferStateFar(s32 owner);
struct BattleUnit *UnitGet(s32 unit_id);

void Func_08118f6c(void)
{
    u8 totals[4];
    u16 party_members[10];
    s32 party_size;
    s32 element;
    u32 available_mask;

    party_size = BattleParty_PrepareActiveOwners(party_members);
    available_mask = Trade_GetOfferStateFar(0)->available_mask & 0xffff0000;

    element = 0;
    do {
        totals[element] = 0;
        {
            s32 party_slot;

            for (party_slot = 0; party_slot < party_size; party_slot++) {
                struct BattleUnit *member = UnitGet(party_members[party_slot]);

                totals[element] += member->djinn_owned_counts[element];
            }
        }
        element++;
    } while (element <= 3);

    element = 0;
    do {
        const struct SummonDefinition *summon = SummonDefinition_Get(element);
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
    } while (element <= 15);

    Trade_GetOfferStateFar(0)->available_mask = available_mask;
}
