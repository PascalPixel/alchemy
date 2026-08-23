#include "types.h"

struct State_0807987c {
    u8 padding[0xf8];
    u8 source[0x30];
    u8 record;
};

struct State_0807987c *Owner_GetState(s32);
s32 Func_080797fc(s32 record, u8 *source, s32 *output);
s32 FixedPoint_Ratio(s32, s32);

s32 Func_0807987c(s32 owner, s32 index)
{
    struct State_0807987c *state = Owner_GetState(owner);
    s32 values[4];
    s32 result = 0;

    if (index <= 3) {
        Func_080797fc(state->record, state->source, values);
        result = FixedPoint_Ratio(values[index], 10);
    }
    return result;
}
