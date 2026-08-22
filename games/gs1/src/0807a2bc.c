#include "types.h"
#include "owner_state.h"

u32 Func_0807a2bc(s32 owner, s32 index, s32 bit)
{
    s32 value =
        ((struct OwnerLearnedState *)Func_08077394(owner))->learned[index] &
        (1 << bit);

    return (u32)(-value | value) >> 31;
}
