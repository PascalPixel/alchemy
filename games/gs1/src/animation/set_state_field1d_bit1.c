#include "types.h"

struct Value_0800c570 {
    u8 padding[29];
    u8 unk_0 : 1;
    u8 field_1 : 1;
    u8 unk_2 : 6;
};

void Animation_SetStateField1dBit1(u8 *obj, u32 v)
{
    if (obj != 0 && obj[84] == 1) {
        struct Value_0800c570 *state = *(struct Value_0800c570 **)(obj + 80);
        state->field_1 = v;
    }
}
