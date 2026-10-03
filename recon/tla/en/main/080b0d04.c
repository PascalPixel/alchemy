/*
 * Earlier private-view trial: 15 bytes differed from +0x2c.
 * EN 2026-10-03: canonical owner fields score 120, 2 differing instructions;
 * compiled and native extents are 84 bytes. The element-offset shift and
 * owner argument are still scheduled later than the listing.
 * Links as recon/tla/raw/080b0d04.s.
 */
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
