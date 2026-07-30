typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

const u8 *Func_08077018(u16);
void Func_080a1f74(s32, u8 *);

/*
 * Reorder the packed fifteen-entry identifier list according to the supplied
 * category sequence. Within each category, select the largest nine-bit value
 * first; criteria with their high bit set accept only identifiers carrying
 * the 0x200 attribute.
 */
s32 Func_080a1e38(u16 *identifiers, s32 ordering) {
    u8 criteria[32];
    u16 remaining[15];
    u16 ordered[16];
    u8 *criterion;
    s32 nonzero_count;
    s32 output_count;
    s32 index;

    Func_080a1f74(ordering, criteria);
    nonzero_count = 0;
    for (index = 0; index < 15; index++) {
        remaining[index] = identifiers[index];
        if (remaining[index] != 0) {
            nonzero_count++;
        }
    }
    for (index = nonzero_count; index < 15; index++) {
        ordered[index] = 0;
    }

    output_count = 0;
    criterion = criteria;
    while (*criterion != 0xFF) {
        for (;;) {
            s32 best_index = 0;
            u16 best_value = 0;

            for (index = 0; index < nonzero_count; index++) {
                u16 identifier = remaining[index];
                u16 value;

                if (identifier == 0) {
                    continue;
                }
                if ((*criterion & 0x7F) != Func_08077018(identifier)[2]) {
                    continue;
                }
                if ((*criterion & 0x80) != 0 &&
                    (identifier & 0x200) == 0) {
                    continue;
                }
                value = identifier & 0x1FF;
                if (best_value < value) {
                    best_value = value;
                    best_index = index;
                }
            }

            if (best_value == 0) {
                break;
            }
            ordered[output_count++] = remaining[best_index];
            remaining[best_index] = 0;
        }
        criterion++;
    }

    for (index = 0; index < nonzero_count; index++) {
        identifiers[index] = ordered[index];
    }
    return 1;
}
