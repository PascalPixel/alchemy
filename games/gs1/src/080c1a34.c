#include "types.h"

struct Record_080c1a34 {
    u8 unknown_00;
    u8 members[5];
    u8 present[5];
    u8 enabled[5];
};

struct Object_080c1a34 {
    u8 padding_00[15];
    u8 value;
};

extern struct Record_080c1a34 Data_080c5c38[];

struct Object_080c1a34 *Func_08077198(s32 id);
s32 Func_080770c0(s32 flag);
s32 Func_080022ec(s32 numerator, s32 denominator);

s32 Func_080c1a34(s32 record_id)
{
    volatile u8 scratch[28];
    struct Record_080c1a34 *record;
    s32 index;
    s32 sum;
    s32 count;

    count = 0;
    sum = 0;
    record = &Data_080c5c38[record_id];
    (void)scratch;

    index = 0;
    if (record->present[0] == 0) {
        u8 *present;

        present = record->present;
        do {
            index++;
            if ((u32)index > 4)
                break;
            present++;
        } while (*present == 0);
    }
    if (index == 5)
        return -1;

    index = 0;
    do {
        if (record->enabled[index] != 0) {
            struct Object_080c1a34 *object;
            s32 member;

            member = record->members[index];
            object = Func_08077198(member + 8);
            if (object != 0) {
                if (object->value <= 3 ||
                    Func_080770c0(372) != 0 ||
                    Func_080770c0(member + 1544) != 0) {
                    sum += object->value;
                    count++;
                } else {
                    return -2;
                }
            }
        }
        index++;
    } while ((u32)index <= 4);

    if (count == 0)
        return -3;
    return Func_080022ec(sum, count);
}
