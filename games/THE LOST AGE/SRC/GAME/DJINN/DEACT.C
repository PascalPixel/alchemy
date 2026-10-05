#include "OWNER_STATE.H"

u32 Djinn_Deactivate(s32 owner, s32 index, s32 bit)
{
    struct OwnerDjinnState *state = Owner_GetState(owner);
    u32 present = Djinn_IsActive(owner, index, bit);

    if (present != 0) {
        state->counts.banks.active[index]--;
        state->flags.banks.active[index] &= ~(1 << bit);
        Owner_RefreshDerivedData(owner);
    }
    return present;
}
