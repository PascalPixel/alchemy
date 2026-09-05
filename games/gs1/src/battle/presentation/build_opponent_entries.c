#include "types.h"
#include "battle_party.h"
#include "battle_runtime.h"
#include "fixed_math.h"

void Func_080bd424(void *entry, s32 retry);
u32 Random16(void);

struct BattlePresentationOpponentEntry {
    s16 unit_id;
    u16 unknown_02;
    u16 value;
    s16 kind;
    s16 parameter;
    s16 unknown_0a;
    u8 unknown_0c[4];
};

s32 Func_080b9324(
    struct BattlePresentationOpponentEntry *entries)
{
    s32 entry_count;
    u8 *mode;
    u16 unit_ids[14];
    s32 unit_count;
    s32 index;
    s32 copy_index;
    s32 unit_id;
    struct BattleUnit *unit;
    struct BattlePresentationOpponentEntry *entry;
    s32 agility;
    s32 limit;
    s32 first;
    s32 second;
    s32 swap;

    entry_count = 0;
    mode = *(u8 **)0x03001e74 + 0x45;
    if (*mode == 1) {
        return 0;
    }

    unit_count = BattleParty_ListLivingUnits(2, unit_ids);
    if (unit_count == 0) {
        return 0;
    }

    index = 31;
    do {
        first = (u32)(unit_count * Random16()) >> 16;
        second = (u32)(unit_count * Random16()) >> 16;
        swap = unit_ids[first];
        unit_ids[first] = unit_ids[second];
        unit_ids[second] = swap;
        index--;
    } while (index >= 0);

    if (*mode == 2) {
        limit = ((u32)(Random16() * 5) >> 16) + 1;
        if (limit <= 1) {
            limit = 2;
        }
        if (limit < unit_count) {
            unit_count = limit;
        }
    }

    for (index = 0; index < unit_count; index++) {
        unit_id = unit_ids[index];
        unit = BattleUnit_Get(unit_id);
        for (copy_index = 0; copy_index < unit->action_entry_count; copy_index++) {
            entry = &entries[entry_count];
            entry->unit_id = unit_id;
            agility = unit->agility;
            entry->value = agility;
            if (copy_index != 0) {
                entry->value = (s16)agility / 2;
            }
            if (unit->sleep != 0 || unit->stun != 0) {
                entry->kind = 8;
                entry->parameter = 0;
                entry->unknown_0a = 0x100;
            } else {
                Func_080bd424(entry, 0);
            }
            entry_count++;
            if (*mode == 2) {
                break;
            }
        }
    }

    return entry_count;
}
