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
s32 GameFlag_IsSet(s32 flag);
s32 FixedPoint_Ratio(s32 numerator, s32 denominator);

s32 Party_ComputeEligibleMemberAverage(s32 record_id)
{
    volatile u8 scratch[28];
    struct Record_080c1a34 *record;
    s32 member_index;
    s32 level_sum;
    s32 eligible_count;

    eligible_count = 0;
    level_sum = 0;
    record = &Data_080c5c38[record_id];
    (void)scratch;

    member_index = 0;
    if (record->present[0] == 0) {
        u8 *present;

        present = record->present;
        do {
            member_index++;
            if ((u32)member_index > 4)
                break;
            present++;
        } while (*present == 0);
    }
    if (member_index == 5)
        return -1;

    member_index = 0;
    do {
        if (record->enabled[member_index] != 0) {
            struct Object_080c1a34 *object;
            s32 member;

            member = record->members[member_index];
            object = Func_08077198(member + 8);
            if (object != 0) {
                if (object->value <= 3 ||
                    GameFlag_IsSet(372) != 0 ||
                    GameFlag_IsSet(member + 1544) != 0) {
                    level_sum += object->value;
                    eligible_count++;
                } else {
                    return -2;
                }
            }
        }
        member_index++;
    } while ((u32)member_index <= 4);

    if (eligible_count == 0)
        return -3;
    return FixedPoint_Ratio(level_sum, eligible_count);
}
