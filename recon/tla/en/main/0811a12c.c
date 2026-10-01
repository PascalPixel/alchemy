/*
 * Draft: BattleParty_ListPresentEnemies, ported from its ☀️ twin; it goes
 * before SRC/BATTLE/PARTY/LIST_LIVING_UNITS.C. Score 60: the listing copies
 * the count to r0 (mov r0, r8) before storing the 0xff terminator; this C
 * stores first. Swapping the two statements and 30 seconds of permuting
 * change nothing.
 */
#include "TYPES.H"
#include "OWNER_STATE.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 GameFlag_Test(s32);

s32 BattleParty_ListPresentEnemies(s16 *unit_ids)
{
    s16 *output;
    s32 battle_result;
    s32 id;
    s32 entry_limit;
    s32 entry_count;

    output = unit_ids;
    entry_count = 0;
    entry_limit = 6;
    battle_result = 0;
    if (output != NULL) {
        if (GameFlag_Test(0x16C) != 0) {
            entry_limit = 3;
        }
        id = 0x80;
        entry_limit += 0x80;
        for (; id < entry_limit; id += 1) {
            if (FIELD_AT_OFFSET(Owner_GetState(id), u8, 0x12A) != 0) {
                *output = (s16)id;
                entry_count += 1;
                output += 1;
            }
        }
        battle_result = entry_count;
        *output = 0xFF;
    }
    return battle_result;
}
