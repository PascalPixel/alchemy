#include "layout_guard.h"
#include "types.h"

typedef struct OwnerMetadata_080b8574 {
    u8 padding_00[0x40];
    u16 weight_40;
} OwnerMetadata_080b8574;

typedef struct Entry_080b8574 {
    u16 owner_00;
    u8 padding_02[2];
    u16 weight_04;
    s16 field_06;
    s16 field_08;
    u16 priority_0a;
    u8 padding_0c[4];
} Entry_080b8574;

LAYOUT_OFFSET_GUARD(
    OwnerMetadata080b8574_Weight,
    OwnerMetadata_080b8574,
    weight_40,
    0x40);
LAYOUT_SIZE_GUARD(Entry080b8574_Size, Entry_080b8574, 0x10);
LAYOUT_OFFSET_GUARD(
    Entry080b8574_Owner,
    Entry_080b8574,
    owner_00,
    0);
LAYOUT_OFFSET_GUARD(
    Entry080b8574_Weight,
    Entry_080b8574,
    weight_04,
    4);
LAYOUT_OFFSET_GUARD(
    Entry080b8574_Field6,
    Entry_080b8574,
    field_06,
    6);
LAYOUT_OFFSET_GUARD(
    Entry080b8574_Field8,
    Entry_080b8574,
    field_08,
    8);
LAYOUT_OFFSET_GUARD(
    Entry080b8574_Priority,
    Entry_080b8574,
    priority_0a,
    0x0a);

u32 Func_08004458(void);
OwnerMetadata_080b8574 *Func_08077008(s32);
s32 Func_080b6b40(s32, u16 *);

/*
 * Build weighted entries for the two owner groups, then perform the original
 * bounded backward sort.  Its total-2 pass budget intentionally leaves lists
 * of two entries untouched.
 */
s32 Func_080b8574(Entry_080b8574 *output)
{
    u16 ids[14];
    s32 party_count = Func_080b6b40(1, ids);
    s32 enemy_count;
    s32 baseline_count;
    s32 total = 0;
    u32 index;

    for (index = 0; index < 4; index++)
        Func_08077008(index);

    for (index = 0; index < party_count; index++) {
        u16 owner = ids[index];
        OwnerMetadata_080b8574 *metadata = Func_08077008(owner);
        Entry_080b8574 *entry = &output[total];

        entry->owner_00 = owner;
        entry->weight_04 = metadata->weight_40;
        entry->field_06 = 0;
        entry->field_08 = 0;
        entry->priority_0a = 0x80;
        total++;
    }

    enemy_count = Func_080b6b40(2, ids);
    baseline_count = Func_080b6b40(1, NULL);

    for (index = 0; index < enemy_count; index++) {
        u16 owner = ids[index];
        OwnerMetadata_080b8574 *metadata = Func_08077008(owner);
        Entry_080b8574 *entry = &output[total];
        u16 base_weight = metadata->weight_40;
        u16 weight = base_weight >> 1;

        entry->owner_00 = owner;
        entry->weight_04 = weight;
        if (weight != 0) {
            entry->weight_04 +=
                (u32)(base_weight * Func_08004458()) >> 16;
        }
        entry->field_06 = 0;
        entry->field_08 = 0;
        entry->priority_0a =
            (u32)(baseline_count * Func_08004458()) >> 16;
        total++;
    }

    {
        s32 pass_budget = total - 2;

        while (pass_budget > 0) {
            s32 swaps = 0;
            s32 entry_number;

            for (entry_number = total - 1;
                 entry_number > 0;
                 entry_number--) {
                Entry_080b8574 *current = &output[entry_number - 1];
                Entry_080b8574 *next = &output[entry_number];

                if ((s16)next->weight_04 >
                    (s16)current->weight_04) {
                    Entry_080b8574 temporary = *next;

                    *next = *current;
                    *current = temporary;
                    swaps++;
                }
            }

            if (swaps == 0)
                break;
            pass_budget--;
        }
    }

    return total;
}
