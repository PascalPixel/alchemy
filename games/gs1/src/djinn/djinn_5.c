#include "types.h"
#include "scene.h"
#include "owner_state.h"

/* djinn/is_active.c */
u32 Djinn_IsActive(s32 owner, s32 index, s32 bit)
{
    s32 value =
        ((struct OwnerLearnedState *)Owner_GetState(owner))->learned[index] &
        (1 << bit);

    return (u32)(-value | value) >> 31;
}

/* djinn/activate.c */
s32 Trade_CanOfferDjinn(s32, s32, s32);
void Owner_RefreshDerivedData(s32 owner);

s32 Djinn_Activate(s32 owner, s32 index, s32 bit)
{
    struct OwnerDjinnState *state =
        (struct OwnerDjinnState *)OwnerState_Get(owner);
    s32 result = Trade_CanOfferDjinn(owner, index, bit);

    if (result != 0) {
        if (state->available[index] & (1 << bit)) {
            state->active[index] |= 1 << bit;
        } else {
            return 0;
        }
        state->active_counts[index]++;
        Owner_RefreshDerivedData(owner);
    }
    return result;
}

/* djinn/deactivate.c */
u32 Djinn_IsActive(s32 owner, s32 index, s32 bit);
void Owner_RefreshDerivedData(s32 owner);

u32 Djinn_Deactivate(s32 owner, s32 index, s32 bit)
{
    struct OwnerDjinnState *state =
        (struct OwnerDjinnState *)OwnerState_Get(owner);
    u32 present = Djinn_IsActive(owner, index, bit);

    if (present != 0) {
        state->active_counts[index]--;
        state->active[index] &= ~(1 << bit);
        Owner_RefreshDerivedData(owner);
    }
    return present;
}
