#include "types.h"

u16 *Func_08004970(s32 size);
s32 Func_080b6b40(s32 side, u16 *out_units);
u8 *Func_08077008(s32 unit_id);
s32 Func_080151c0(void *entries, u16 *excluded_units, s32 excluded_count);
void Func_08002df0(void *ptr);

#define BattlePresentation_BuildUnitEntries Func_080b920c

struct BattlePresentationUnitEntry {
    u16 unit_id;
    u16 unknown_02;
    u16 value;
    s16 width;
    s16 mode;
    s16 height;
    u8 unknown_0c[4];
};

s32 BattlePresentation_BuildUnitEntries(
    struct BattlePresentationUnitEntry *entries)
{
    u16 *excluded_units = Func_08004970(17);
    u16 *unit_ids = Func_08004970(9);
    s32 unit_count = Func_080b6b40(1, unit_ids);
    s32 entry_count = 0;
    s32 excluded_count = 0;
    s32 unit_index;

    for (unit_index = 0; unit_index < unit_count; unit_index++) {
        u16 unit_id = unit_ids[unit_index];
        u8 *unit = Func_08077008(unit_id);
        s32 copy_count = unit[0x43];
        s32 copy_index;

        for (copy_index = 0; copy_index < copy_count; copy_index++) {
            if (unit[0x13c] != 0 || (*(u32 *)(unit + 0x138) & 0xffffff00)) {
                struct BattlePresentationUnitEntry *entry =
                    &entries[entry_count++];
                entry->unit_id = unit_id;
                entry->value = *(u16 *)(unit + 0x40);
                entry->width = 8;
                entry->mode = 0;
                entry->height = 0x180;
            } else {
                excluded_units[excluded_count++] = unit_id;
            }
        }
    }

    {
        s32 appended = Func_080151c0(
            &entries[entry_count],
            excluded_units,
            excluded_count);
        s32 result = -1;

        if (appended >= 0) {
            result = entry_count + appended;
        }
        Func_08002df0(unit_ids);
        Func_08002df0(excluded_units);
        return result;
    }
}
