#include "PROTO.H"
#include "OWNER_STATE.H"

u32 Djinn_IsActive(s32 owner, s32 index, s32 bit)
{
    s32 value =
        ((struct OwnerDjinnState *)Owner_GetState(owner))->flags.banks.active[index] &
        (1 << bit);

    return (u32)(-value | value) >> 31;
}


s32 Djinn_Activate(s32 owner, s32 index, s32 bit)
{
    struct OwnerDjinnState *state = Owner_GetState(owner);
    s32 result = Trade_CanOfferDjinn(owner, index, bit);

    if (result != 0) {
        u32 owned = state->flags.banks.owned[index];
        if (owned & (1 << bit)) {
            state->flags.banks.active[index] |= 1 << bit;
        } else {
            return 0;
        }
        state->counts.banks.active[index]++;
        Owner_RefreshDerivedData(owner);
    }
    return result;
}

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
