#include "OWNER_STATE.H"

void Owner_RefreshDerivedData(s32 owner);

u32 Djinn_IsActive(s32 owner, s32 index, s32 bit);
void Owner_RefreshDerivedData(s32 owner);

u32 Djinn_IsActive(s32 owner, s32 index, s32 bit);
u32 Djinn_Deactivate(s32 owner, s32 index, s32 bit);

u32 Djinn_Deactivate(s32 owner, s32 index, s32 bit)
{
    struct OwnerDjinnState *state =
        (struct OwnerDjinnState *)Owner_GetState(owner);
    u32 present = Djinn_IsActive(owner, index, bit);

    if (present != 0) {
        state->active_counts[index]--;
        state->active[index] &= ~(1 << bit);
        Owner_RefreshDerivedData(owner);
    }
    return present;
}
