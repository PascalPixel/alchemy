#include "types.h"

struct State {
    u8 field0;
    u8 field1;
    u8 field2;
    u8 field3;
    u8 filler4[5];
    u8 field9;
    u8 filler10;
    u8 field11;
};

u8 Func_0800615c(s32 arg0);
void Func_080060e8(s32 arg0);

s32 Func_08006088(s32 arg0, s32 arg1)
{
    u32 control = *(u32 *)0x04000128;
    struct State *state = (struct State *)0x02002240;
    s32 packed;
    s32 result;

    if (state->field1 == 1) {
        Func_0800615c(arg1);
        Func_080060e8(arg0);
        state->field11++;
    }
    packed = state->field3;
    packed |= state->field2 << 8;
    if (state->field0 == 8)
        packed |= 0x80;
    result = packed;
    if (state->field9 != 0)
        result |= 0x1000;
    if (((control << 26) >> 30) > 1)
        result |= 0x2000;
    return result;
}
