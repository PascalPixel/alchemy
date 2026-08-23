#include "types.h"

struct State_0807a350 {
    u8 padding[0x108];
    u32 flags[5];
    u8 counts[5];
};

struct State_0807a350 *Owner_GetState(s32);
u32 Func_0807a2bc(s32 owner, s32 index, s32 bit);
void Func_08079ae8(s32 arg0);

u32 Func_0807a350(s32 owner, s32 index, s32 bit)
{
    struct State_0807a350 *state = Owner_GetState(owner);
    u32 present = Func_0807a2bc(owner, index, bit);

    if (present != 0) {
        state->counts[index]--;
        state->flags[index] &= ~(1 << bit);
        Func_08079ae8(owner);
    }
    return present;
}
