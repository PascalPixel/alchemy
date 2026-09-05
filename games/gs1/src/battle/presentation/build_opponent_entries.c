#include "types.h"

s32 Func_080b6b40(s32 side, u16 *out_units);
u32 Func_08004458(void);
u8 *Func_08077008(s32 unit_id);
void Func_080bd424(void *entry, s32 arg1);


struct BattlePresentationOpponentEntry {
    u16 unit_id;
    u16 unknown_02;
    u16 value;
    s16 width;
    s16 mode;
    s16 height;
    u8 unknown_0c[4];
};

s32 BattlePresentation_BuildOpponentEntries(
    struct BattlePresentationOpponentEntry *entries)
{
    u16 unit_ids[14];
    s32 entry_count = 0;
    u8 *battle = *(u8 **)0x03001e74;
    s32 unit_count;
    s32 i;

    if (battle[0x45] == 1) {
        return 0;
    }

    unit_count = Func_080b6b40(2, unit_ids);
    if (unit_count == 0) {
        return 0;
    }

    for (i = 31; i >= 0; i--) {
        u32 first = (u32)(unit_count * Func_08004458()) >> 16;
        u32 second = (u32)(unit_count * Func_08004458()) >> 16;
        s32 swap = unit_ids[first];
        unit_ids[first] = unit_ids[second];
        unit_ids[second] = swap;
    }

    if (battle[0x45] == 2) {
        s32 limit = ((u32)(Func_08004458() * 5) >> 16) + 1;

        if (limit <= 1) {
            limit = 2;
        }
        if (limit < unit_count) {
            unit_count = limit;
        }
    }

    for (i = 0; i < unit_count; i++) {
        s32 unit_id = unit_ids[i];
        u8 *unit = Func_08077008(unit_id);
        s32 copy_index;

        for (copy_index = 0; copy_index < unit[0x43]; copy_index++) {
            struct BattlePresentationOpponentEntry *entry =
                &entries[entry_count];
            s32 value;

            entry->unit_id = unit_id;
            value = *(u16 *)(unit + 0x40);
            entry->value = value;
            if (copy_index != 0) {
                entry->value = (s16)value / 2;
            }

            if (unit[0x13c] != 0 || unit[0x13b] != 0) {
                entry->width = 8;
                entry->mode = 0;
                entry->height = 0x100;
            } else {
                Func_080bd424(entry, 0);
            }

            entry_count++;

            if (battle[0x45] == 2) {
                break;
            }
        }
    }

    return entry_count;
}
