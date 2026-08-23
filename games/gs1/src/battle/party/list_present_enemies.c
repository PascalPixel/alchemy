#include "battle_party.h"
#include "battle_runtime.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 BattleParty_ListPresentEnemies(s16 *unit_ids) {
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
        if (BattleFlag_Test(0x16C) != 0) {
            entry_limit = 3;
        }
        id = 0x80;
        entry_limit += 0x80;
        for (; id < entry_limit; id += 1) {
            if (M2C_FIELD(BattleUnit_Get(id), u8 *, 0x12A) != 0) {
                *output = (s16) id;
                entry_count += 1;
                output += 1;
            }
        }
        *output = 0xFF;
        battle_result = entry_count;
    }
    return battle_result;
}
