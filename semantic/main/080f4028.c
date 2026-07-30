#include "types.h"

struct State_080f4028 {
    u8 filler0[12];
    s32 field0c;
    s32 field10;
    s32 field14;
    s32 field18;
    s32 field1c;
    s32 field20;
    u8 filler24[16];
    s16 field34;
    s16 field36;
};

struct Local_080f4028 {
    s32 first;
    s32 second;
    s32 third;
};

extern struct State_080f4028 *Data_03001e80;
extern s32 Data_03001ce0[];

void Func_080049ac(void);
void Func_08004cb4(void *);
void Func_08004c1c(s32);
void Func_08004bd4(s32);
void Func_08005258(u32, u32, u32);

void Func_080f4028(void)
{
    struct State_080f4028 *state = Data_03001e80;
    struct Local_080f4028 local;
    s32 result;

    state->field34 = 152 << 8;
    state->field20 = 255 << 17;
    state->field0c = 0;
    state->field10 = 0;
    state->field14 = 0;
    state->field36 = 0;
    state->field1c = 0;
    Data_03001ce0[3] = 0;
    Data_03001ce0[4] = 0;
    state->field18 = 0;

    Func_080049ac();
    Func_08004cb4(&state->field0c);
    Func_08004c1c(state->field36);
    Func_08004bd4(state->field34);

    local.first = 0;
    local.second = 0;
    local.third = state->field20;
    ((void (*)(struct Local_080f4028 *, struct State_080f4028 *))
        0x03000250)(&local, state);

    result = ((s32 (*)(s32, s32))0x0300013c)(250 << 16, 192 << 8);
    Func_08005258(250 << 16, result, 0x7fff0000);
}
