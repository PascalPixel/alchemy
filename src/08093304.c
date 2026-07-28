#include "types.h"

struct State08093304 {
    u8 padding[0x12F4];
    s16 result;
    s16 value;
};

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s32 Func_08092ba8(s32 key);
s32 Func_080915ac(u32 id);
extern u8 Data_02000240[];
extern u8 Data_0809fc28[];
extern struct State08093304 *Data_03001e8c;

void Func_08093304(s32 arg0)
{
    struct State08093304 *state = Data_03001e8c;
    u8 value;
    s32 result;

    if (arg0 == (s32)0x80000000) {
        FIELD(state, s16, 0x12F4) = (value = 0);
        FIELD(state, s16, 0x12F6) = value;
    } else {
        result = Func_080915ac(Func_08092ba8(arg0));
        value = Data_0809fc28[Data_02000240[0x20C]];
        FIELD(state, s16, 0x12F4) = result;
        FIELD(state, s16, 0x12F6) = value;
    }
}
