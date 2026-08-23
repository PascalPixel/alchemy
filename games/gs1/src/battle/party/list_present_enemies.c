#include "battle_party.h"
#include "battle_runtime.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 BattleParty_ListPresentEnemies(s16 *unit_ids) {
    s16 *output;
    s32 result;
    s32 id;
    s32 limit;
    s32 count;

    output = unit_ids;
    count = 0;
    limit = 6;
    result = 0;
    if (output != NULL) {
        if (BattleFlag_Test(0x16C) != 0) {
            limit = 3;
        }
        id = 0x80;
        limit += 0x80;
        for (; id < limit; id += 1) {
            if (M2C_FIELD(BattleUnit_Get(id), u8 *, 0x12A) != 0) {
                *output = (s16) id;
                count += 1;
                output += 1;
            }
        }
        *output = 0xFF;
        result = count;
    }
    return result;
}
