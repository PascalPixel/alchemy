/*
 * Draft: BattleEscape_CheckSuccess, ported from its ☀️ twin; it follows
 * BattleUnit_ClearField12bForGroup in SRC/BATTLE/BATTLE2.C. Score 200: the
 * listing sets the first ListLivingUnits argument (movs r0, #1) and the
 * first level total (movs r6, #0) before the chance is computed, where this
 * sets them after; and it loads gPartyState's address before the split
 * constant 0x24b, where this builds the constant first.
 */
#include "TYPES.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_ESCAPE.H"
#include "OWNER_STATE.H"
#include "PARTY_STATE.H"
#include "RAM_BUFFER.H"

s32 __divsi3(s32, s32);
u32 Random16(void);

struct BattleEscapeState {
    u8 reserved_00[0x45];
    u8 guaranteed;
    u8 failed_attempts;
};

s32 BattleEscape_CheckSuccess(void)
{
    s32 escaped;
    u8 *failed_attempts;
    s16 living_units[14];
    s32 living_count;
    s32 level_total;
    s32 unit_index;
    s32 chance;
    struct BattleEscapeState *escape_state;

    escaped = 0;
    escape_state = (struct BattleEscapeState *)Ram_HeapSlots->battle_work;
    if (escape_state->guaranteed == 1) {
        escaped = 1;
    } else {
        failed_attempts = &escape_state->failed_attempts;
        level_total = 0;
        chance = 0x1388 + (escape_state->failed_attempts * 0x7D0);
        living_count = BattleParty_ListLivingUnits(
            BATTLE_SIDE_PARTY,
            living_units);
        for (unit_index = escaped; unit_index < living_count; unit_index++) {
            level_total += ((u8 *)Owner_GetState(
                (s32)living_units[unit_index]))[0x0f];
        }
        chance += __divsi3(level_total * 0x1F4, living_count);
        living_count = BattleParty_ListLivingUnits(
            BATTLE_SIDE_ENEMIES,
            living_units);
        level_total = 0;
        for (unit_index = 0; unit_index < living_count; unit_index++) {
            level_total += ((u8 *)Owner_GetState(
                (s32)living_units[unit_index]))[0x0f];
        }
        chance -= __divsi3(level_total * 0x1F4, living_count);
        if ((chance > 0) &&
            ((u32)((u32)(0x2710 * Random16()) >> 0x10) < (u32)chance)) {
            escaped = 1;
        }
        *failed_attempts += 1;
    }
    if (gPartyState.battle_rule_24b == 2) {
        escaped = 0;
    }
    return escaped;
}
