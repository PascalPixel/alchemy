#include "types.h"

struct State_0807a2e4 {
    u8 padding[0xF8];
    u32 available[4];
    u32 flags[5];
    u8 counts[5];
};

struct State_0807a2e4 *Owner_GetState(s32);
s32 Func_0807a1f8(s32, s32, s32);
void Func_08079ae8(s32 arg0);

s32 Func_0807a2e4(s32 owner, s32 index, s32 bit)
{
    struct State_0807a2e4 *state = Owner_GetState(owner);
    s32 result = Func_0807a1f8(owner, index, bit);

    if (result != 0) {
        if (state->available[index] & (1 << bit)) {
            state->flags[index] |= 1 << bit;
        } else {
            return 0;
        }
        state->counts[index]++;
        Func_08079ae8(owner);
    }
    return result;
}
