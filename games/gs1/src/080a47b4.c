#include "types.h"

struct State_080a47b4 {
    u8 filler00[48];
    s32 object;
    u8 filler34[324];
    u16 values[0x100];
};

extern struct State_080a47b4 *Data_03001f2c;

s32 Func_080a10d0(s32 *, s32, s32, s32, s32, s32);
void Func_080a22f4(void);
void Func_080a4924(s32, u16);

s32 Func_080a47b4(s32 index)
{
    struct State_080a47b4 *state = Data_03001f2c;

    Func_080a10d0(&state->object, 0, 0, 13, 10, 2);
    Func_080a22f4();

    if (state->values[index] != 0)
        Func_080a4924(state->object, state->values[index]);

    return 1;
}
