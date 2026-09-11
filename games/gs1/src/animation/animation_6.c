#include "types.h"
#include "scene.h"

/* animation/set_state_field5_bits2_3.c */
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

/* animation/set_state_field1d_bit1.c */
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

/* animation/apply_child_values.c */
void Animation_ApplyChildValues(void *obj)
{
    if ((obj != NULL) && (FIELD_AT_OFFSET(obj, u8 *, 0x54) == 1)) {
        Sys_Check(FIELD_AT_OFFSET(obj, s32 *, 0x50));
    }
}
