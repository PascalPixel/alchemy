#include "types.h"

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

struct EffectBlockState {
    u8 filler[0x1F80];
    u16 field_1f80;
    u16 field_1f82;
};

void BattleFx_SetBlock30ValuesMaxZero(void)
{
    struct EffectBlockState *state = Runtime_AllocateBlock(30, 0x1F88);
    state->field_1f80 = 0x7FFF;
    state->field_1f82 = 0;
}

void BattleFx_SetBlock30Values12Zero(void)
{
    struct EffectBlockState *state = Runtime_AllocateBlock(30, 0x1F88);
    state->field_1f80 = 12;
    state->field_1f82 = 0;
}

void BattleFx_SetBlock30Values128One(void)
{
    struct EffectBlockState *state = Runtime_AllocateBlock(30, 0x1F88);
    state->field_1f80 = 128;
    state->field_1f82 = 1;
}
