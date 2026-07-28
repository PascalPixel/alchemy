#include "types.h"
#include "owner_state.h"

s32 Func_080784b0(s32 owner, s32 index)
{
    s32 masked;

    owner = ((struct OwnerInventoryState *)Func_08077394(owner))->inventory[index];
    masked = 0x1ff;
    masked &= owner;
    owner = (u32)owner >> 11;
    owner++;
    if (masked == 0) {
        owner = 0;
    }
    return owner;
}
