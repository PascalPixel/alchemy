#include "types.h"

#define Animation_SetStateField5Bits2To3 Func_0800c548

struct Value_0800c548 {
    u8 padding[5];
    u8 unk_0 : 2;
    u8 field_2 : 2;
    u8 unk_4 : 4;
};

void Animation_SetStateField5Bits2To3(u8 *obj, u32 v)
{
    if (obj != 0 && obj[84] == 1) {
        struct Value_0800c548 *state = *(struct Value_0800c548 **)(obj + 80);
        state->field_2 = v;
    }
}
