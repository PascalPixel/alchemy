#include "metadata_lookup.h"
#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* ui/icon/find_next_number_with_metadata.c */
s32 Ui_FindNextNumberWithMetadata(s32 start, s32 step)
{
    s32 value = start;
    s32 delta = step + value - value;

    for (;;) {
        value += delta;
        if (value < 0) {
            value = 0x200;
            continue;
        }
        {
            s32 limit = 0x200;
            if (value >= limit) {
                value = -1;
                continue;
            }
        }
        if (*(u8 *)Ui_Run(value)) {
            return value;
        }
    }
}

/* ui/icon/set_grid_column_byte5.c */
extern u8 *gIw;

void Ui_SetGridColumnByte5(s32 slot, s32 value)
{
    u8 *base = gIw;
    s32 offset = (slot & 3) * 4 + 40;
    s32 count = 9;

    do {
        u8 *entry = *(u8 **)(base + offset);

        count--;
        entry[5] = value;
        base += 56;
    } while (count >= 0);
}

/* ui/icon/set_grid_column_byte6.c */

void Ui_SetGridColumnByte6(s32 slot, s32 value)
{
    u8 *base = gIw;
    s32 offset = (slot & 3) * 4 + 40;
    s32 count = 9;

    do {
        u8 *entry = *(u8 **)(base + offset);

        count--;
        entry[6] = value;
        base += 56;
    } while (count >= 0);
}

/* ui/icon/fill_grid_column_from_metadata.c */
struct Entry_08012d70 {
    s16 no;
    s16 x;
    u8 value_04;
    u8 unknown_05[7];
    s32 *table_0c;
    s32 value_10;
    s8 value_14;
    s8 value_15;
    u8 value_16;
    s8 value_17;
};

void Ui_FillGridColumnFromMetadata(s32 slot, s32 value)
{
    s32 index;
    s32 count;
    s32 offset;
    u8 *metadata;
    struct Entry_08012d70 *entry;
    u8 *work;

    work = *(u8 **)ADDR_03001E60;
    count = 0;
    offset = ((3 & slot) * 4) + 0x28;
    index = 0;
    do {
        entry = *(struct Entry_08012d70 **)(work + offset);
        if (entry->table_0c != 0) {
            metadata = Ui_Run(entry->no);
            if (value < metadata[5]) {
                entry->value_04 = metadata[4];
                entry->value_10 = entry->table_0c[value];
                entry->x = count * 0x10;
                entry->value_15 = 0x10;
                entry->value_14 = index;
                entry->value_17 = index;
                entry->value_16 = 0xFF;
            }
            work[0x23] = metadata[7];
            *(s16 *)(work + 0x1e) = index;
        }
        count += 1;
        work += 0x38;
    } while (count <= 9);
}

/* ui/icon/set_grid_column_number.c */

void Ui_SetGridColumnNumber(s32 slot, s32 no)
{
    u8 *base = gIw;
    s32 offset;
    s32 count;

    Ui_Run(no);
    offset = (slot & 3) * 4 + 40;
    count = 9;
    do {
        u8 *entry = *(u8 **)(base + offset);

        count--;
        *(u16 *)entry = no;
        Ui_Do(entry);
        base += 56;
    } while (count >= 0);
}
