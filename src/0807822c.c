#include "owner_state.h"
#include "types.h"

s32 Func_080022ec(s32, s32);

struct Owner_0807822c {
    u8 unknown_00[0x14];
    s16 value_14;
    s16 value_16;
    u8 unknown_18[0x1c];
    s16 divisor_34;
    s16 divisor_36;
    s16 value_38;
    s16 value_3a;
};

void Func_0807822c(s32 owner_no)
{
    s32 first;
    s32 second;
    s32 first_value;
    s32 second_value;
    struct Owner_0807822c *owner;

    owner = Func_08077394(owner_no);
    first = Func_080022ec(
        (s32)((u32)(s32)owner->value_38 << 14), owner->divisor_34);
    first_value = 0x4000;
    if (first <= 0x4000) {
        first_value = 0;
        if (first >= 0) {
            first_value = first;
        }
    }
    owner->value_14 = first_value;
    if ((((u32)first_value << 16) == 0) && (owner->value_38 != 0)) {
        first_value = 1;
        owner->value_14 = first_value;
    }
    second = Func_080022ec(
        (s32)((u32)(s32)owner->value_3a << 14), owner->divisor_36);
    second_value = 0x4000;
    if (second <= 0x4000) {
        second_value = 0;
        if (second >= 0) {
            second_value = second;
        }
    }
    owner->value_16 = second_value;
    if ((((u32)second_value << 16) == 0) && (owner->value_3a != 0)) {
        second_value = 1;
        owner->value_16 = second_value;
    }
}
