#include "types.h"

struct ListEntry_080228e4 {
    u16 id;
    u16 unknown_02;
};

struct ActionInfo_080228e4 {
    u8 unknown_00;
    u8 flags;
};

struct ActionInfo_080228e4 *Func_08077080(s32);

/*
 * Merge eligible IDs from two 32-entry lists. IDs found only in the second
 * list receive bit 15; IDs found only in the first receive bit 14.
 */
s32 Func_080228e4(
    const struct ListEntry_080228e4 *first,
    const struct ListEntry_080228e4 *second,
    u16 *output,
    s32 *second_only_count,
    s32 *first_only_count)
{
    s32 output_count;
    s32 second_only;
    s32 first_only;
    u32 index;

    output_count = 0;
    second_only = 0;
    first_only = 0;

    index = 0;
    while (index < 32 && second[index].id != 0) {
        u16 id = second[index].id;
        struct ActionInfo_080228e4 *action = Func_08077080((s32)id);

        if ((action->flags & 0x80) != 0) {
            u32 match;

            output[output_count] = id & 0x3FFF;
            output_count++;

            match = 0;
            while (match < 32) {
                if (((id ^ first[match].id) & 0x3FFF) == 0) {
                    break;
                }
                match++;
            }
            if (match == 32) {
                second_only++;
                output[output_count - 1] |= 0x8000;
            }
        }
        index++;
    }

    index = 0;
    while (index < 32 && first[index].id != 0) {
        u16 id = first[index].id;
        struct ActionInfo_080228e4 *action = Func_08077080((s32)id);

        if ((action->flags & 0x80) != 0) {
            u32 match;

            match = 0;
            while (match < 32) {
                if (((id ^ second[match].id) & 0x3FFF) == 0) {
                    break;
                }
                match++;
            }
            if (match == 32) {
                first_only++;
                output[output_count] = (id & 0x3FFF) | 0x4000;
                output_count++;
            }
        }
        index++;
    }

    *second_only_count = second_only;
    *first_only_count = first_only;
    return output_count;
}
