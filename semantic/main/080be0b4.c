#include "types.h"

struct PendingEntry_080be0b4 {
    u8 category;
    u8 unknown_01;
    u8 object_id;
    s8 state;
};

struct PendingTable_080be0b4 {
    u8 unknown_000[8];
    struct PendingEntry_080be0b4 entries[64];
    s32 count;
};

s32 Func_080b6c08(s32 groups, s16 *object_ids);
struct PendingTable_080be0b4 *Func_08077000(s32 side);

/*
 * Count pending entries belonging to the selected side's active objects.
 * Optional category_counts receives the four per-category totals.
 */
s32 Func_080be0b4(u32 object_id, u8 *category_counts)
{
    s16 active_ids[8];
    s32 active_count;
    struct PendingTable_080be0b4 *table;
    s32 result = 0;
    s32 index;

    active_count = Func_080b6c08(object_id > 7 ? 2 : 1, active_ids);
    table = Func_08077000(object_id > 7 ? 1 : 0);

    if (category_counts != 0) {
        for (index = 0; index < 4; index++) {
            category_counts[index] = 0;
        }
    }

    for (index = 0; index < table->count; index++) {
        struct PendingEntry_080be0b4 *entry = &table->entries[index];

        if (entry->state == -1) {
            s32 active_index;

            for (active_index = 0;
                 active_index < active_count;
                 active_index++) {
                if (active_ids[active_index] == entry->object_id) {
                    break;
                }
            }

            if (active_index != active_count) {
                if (category_counts != 0) {
                    category_counts[entry->category]++;
                }
                result++;
            }
        }
    }

    return result;
}
