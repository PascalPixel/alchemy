#include "types.h"
#include "battle_party.h"
#include "battle_escape.h"
#include "owner_state.h"
#include "party_state.h"
#include "ram_buffer.h"

s32 __divsi3(s32, s32);
u32 Random16(void);

struct BattleEscapeState {
    u8 reserved00[0x45];
    u8 guaranteed;
    u8 failedAttempts;
};

s32 BattleEscape_CheckSuccess(void)
{
    s32 escaped;
    u8 *failedAttempts;
    s16 living_units[14];
    s32 living_count;
    s32 level_total;
    s32 unit_index;
    s32 chance;
    struct BattleEscapeState *escape_state;

    escaped = 0;
    escape_state = (struct BattleEscapeState *)Ram_HeapSlots->battleWork;
    if (escape_state->guaranteed == 1) {
        escaped = 1;
    } else {
        failedAttempts = &escape_state->failedAttempts;
        level_total = 0;
        chance = 0x1388 + (escape_state->failedAttempts * 0x7D0);
        living_count = BattleParty_ListLivingUnits(
            BATTLE_SIDE_PARTY,
            living_units);
        for (unit_index = escaped; unit_index < living_count; unit_index++) {
            level_total += ((u8 *)UnitGet(
                (s32)living_units[unit_index]))[0x0f];
        }
        chance += __divsi3(level_total * 0x1F4, living_count);
        living_count = BattleParty_ListLivingUnits(
            BATTLE_SIDE_ENEMIES,
            living_units);
        level_total = 0;
        for (unit_index = 0; unit_index < living_count; unit_index++) {
            level_total += ((u8 *)UnitGet(
                (s32)living_units[unit_index]))[0x0f];
        }
        chance -= __divsi3(level_total * 0x1F4, living_count);
        if ((chance > 0) &&
            ((u32)((u32)(0x2710 * Random16()) >> 0x10) < (u32)chance)) {
            escaped = 1;
        }
        *failedAttempts += 1;
    }
    if (gPartyState.battleRule24b == 2) {
        escaped = 0;
    }
    return escaped;
}
