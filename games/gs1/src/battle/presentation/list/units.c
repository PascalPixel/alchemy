#include "types.h"
#include "scene.h"
#include "abi/battle/presentation/list/units.h"

u16 *Battle_Run(s32 size);

u8 *Battle_Run2(s32 unit_id);

struct BattlePresentationUnitEntry {
    u16 unit_id;
    u16 unknown_02;
    u16 value;
    s16 width;
    s16 mode;
    s16 height;
    u8 unknown_0c[4];
};

s32 BattlePres_BuildUnitEntries(
    struct BattlePresentationUnitEntry *entries)
{
    u16 *excluded_units = Battle_Run(17);
    u16 *unit_ids = Battle_Run(9);
    s32 unit_count = Battle_Apply(1, unit_ids);
    s32 excluded_count = 0;
    s32 entry_count = 0;
    s32 unit_index;

    for (unit_index = 0; unit_index < unit_count; unit_index++) {
        u8 *unit = Battle_Run2(unit_ids[unit_index]);
        s32 copy_index;

        for (copy_index = 0; copy_index < unit[0x43]; copy_index++) {
            if (unit[0x13c] != 0 || (*(u32 *)(unit + 0x138) & 0xffffff00)) {
                struct BattlePresentationUnitEntry *entry =
                    &entries[entry_count];
                entry->unit_id = unit_ids[unit_index];
                entry->value = *(u16 *)(unit + 0x40);
                entry->width = 8;
                entry->mode = 0;
                entry->height = 0x180;
                entry_count++;
            } else {
                excluded_units[excluded_count++] = unit_ids[unit_index];
            }
        }
    }

    {
        s32 appended;

        entries += entry_count;
        appended = Battle_Place(entries, excluded_units, excluded_count);
        if (appended < 0) {
            unit_count = -1;
        } else {
            unit_count = entry_count + appended;
        }
        Battle_Do(unit_ids);
        Battle_Do(excluded_units);
        return unit_count;
    }
}
