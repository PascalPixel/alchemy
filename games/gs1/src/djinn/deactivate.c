#include "types.h"
#include "owner_state.h"

u32 Djinn_IsActive(s32 owner, s32 index, s32 bit);
void Func_08079ae8(s32 arg0);

u32 Djinn_Deactivate(s32 owner, s32 index, s32 bit)
{
    struct OwnerDjinnState *state =
        (struct OwnerDjinnState *)OwnerState_Get(owner);
    u32 present = Djinn_IsActive(owner, index, bit);

    if (present != 0) {
        state->active_counts[index]--;
        state->active[index] &= ~(1 << bit);
        Func_08079ae8(owner);
    }
    return present;
}
