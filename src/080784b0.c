#include "types.h"

struct State_080784b0 {
    u8 padding[216];
    u16 entries[15];
};

struct State_080784b0 *Func_08077394(s32);

s32 Func_080784b0(s32 owner, s32 index)
{
    s32 masked;

    owner = Func_08077394(owner)->entries[index];
    masked = 0x1ff;
    masked &= owner;
    owner = (u32)owner >> 11;
    owner++;
    if (masked == 0) {
        owner = 0;
    }
    return owner;
}
