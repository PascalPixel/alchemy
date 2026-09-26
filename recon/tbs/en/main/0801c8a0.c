/* NONMATCHING: 132-byte owner and pool; 33 differing halfwords. The loops
 * have the reference's shape but output, table and counter registers differ.
 * Widening the loaded u16 primary adds sign/zero conversions and rotates
 * the loops (152 bytes); preserve the narrow scalar and stop that axis. */
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

    index = 0;
    do {
        primary = Data_02000462;
        if (entries->group == (primary & 0x3ff) &&
            entries->value == (primary >> 10)) {
            *second_match = index;
            break;
        }
        index++;
        entries++;
    } while (index <= 447);
}
