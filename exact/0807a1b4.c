#include "types.h"
#include "owner_state.h"

s32 Func_0807a1b4(s32 owner, s32 index, s32 bit)
{
    struct OwnerBitState *state = Func_08077394(owner);

    if (state->bit_counts[index] > 9)
        return -1;
    if ((state->bits[index] & (1 << bit)) != 0)
        return -1;
    state->bit_counts[index]++;
    state->bits[index] |= 1 << bit;
    return 0;
}
