/* NONMATCHING: complete 132-byte owner and pool; candidate 132 bytes,
 * 30 differing halfwords / 30 aligned edits (2026-09-26).
 * Phase-local widening of only the second sampled value left the earlier
 * 132/33/33 baseline unchanged. Independent scan counters recover the
 * first loop counter in r1 (retained: 132/30/30). An explicit shared table
 * cursor grows to 160 bytes / 78 halfwords / 58 edits and changes topology.
 * Remaining: output/table/mask registers and the second loop's widened
 * shift/reload. Three bounded hypotheses used; no credit until exact.
 * Earlier widening of both samples produced 152 bytes and rotated loops. */
#include "TYPES.H"

struct EncodedIndexEntry {
    u16 value;
    u16 group;
};

struct EncodedIndexRuntime {
    u32 prefix[136];
    volatile u16 primary;
};

extern struct EncodedIndexRuntime Data_02000240;
extern volatile u16 Data_02000462;

void Func_0801c8a0(u32 *first_match, u32 *second_match,
                             const struct EncodedIndexEntry *entries)
{
    s32 index;
    s32 second_index;
    u16 primary;

    *first_match = 0;
    *second_match = 0;

    primary = Data_02000240.primary;
    for (index = 0; index <= 447; index++) {
        if (entries[index].group == (primary & 0x3ff) &&
            entries[index].value == (primary >> 10)) {
            *first_match = index;
            break;
        }
    }

    second_index = 0;
    do {
        u32 secondary;

        secondary = Data_02000462;
        if (entries->group == (secondary & 0x3ff) &&
            entries->value == (secondary >> 10)) {
            *second_match = second_index;
            break;
        }
        second_index++;
        entries++;
    } while (second_index <= 447);
}
