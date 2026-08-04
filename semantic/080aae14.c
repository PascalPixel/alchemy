#include "layout_guard.h"
#include "types.h"

enum {
    COMPARISON_SLOT_COUNT_080AAE14 = 32,
    COMPARISON_ID_MASK_080AAE14 = 0x3fff,
    COMPARISON_REMOVED_080AAE14 = 0x4000,
    COMPARISON_ADDED_080AAE14 = 0x8000,
};

typedef struct ComparisonSlot_080aae14 {
    u16 value;
    u16 metadata;
} ComparisonSlot_080aae14;

LAYOUT_SIZE_GUARD(
    ComparisonSlot080aae14_Size,
    ComparisonSlot_080aae14,
    4);

static s32 ContainsId_080aae14(
    const ComparisonSlot_080aae14 *slots,
    u16 value)
{
    s32 index;
    u16 id = value & COMPARISON_ID_MASK_080AAE14;

    for (index = 0; index < COMPARISON_SLOT_COUNT_080AAE14; index++) {
        if ((slots[index].value & COMPARISON_ID_MASK_080AAE14) == id)
            return 1;
    }

    return 0;
}

/*
 * Build the display list that compares a previous 32-slot set with its current
 * value.  Every current ID is emitted first; IDs absent from the previous set
 * carry 0x8000.  Previous IDs no longer present are appended with 0x4000.
 */
s32 Func_080aae14(
    u16 *previous_values,
    u16 *current_values,
    s16 *output_values,
    s32 *out_added,
    s32 *out_removed)
{
    const ComparisonSlot_080aae14 *previous =
        (const ComparisonSlot_080aae14 *)previous_values;
    const ComparisonSlot_080aae14 *current =
        (const ComparisonSlot_080aae14 *)current_values;
    u16 *output = (u16 *)output_values;
    s32 output_count = 0;
    s32 added = 0;
    s32 removed = 0;
    s32 index;

    for (index = 0;
         index < COMPARISON_SLOT_COUNT_080AAE14 && current[index].value != 0;
         index++) {
        u16 encoded = current[index].value & COMPARISON_ID_MASK_080AAE14;

        if (!ContainsId_080aae14(previous, current[index].value)) {
            encoded |= COMPARISON_ADDED_080AAE14;
            added++;
        }
        output[output_count++] = encoded;
    }

    for (index = 0;
         index < COMPARISON_SLOT_COUNT_080AAE14 && previous[index].value != 0;
         index++) {
        if (!ContainsId_080aae14(current, previous[index].value)) {
            output[output_count++] =
                (previous[index].value & COMPARISON_ID_MASK_080AAE14) |
                COMPARISON_REMOVED_080AAE14;
            removed++;
        }
    }

    *out_added = added;
    *out_removed = removed;
    return output_count;
}
