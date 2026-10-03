/*
 * Earlier private-view trial: 12 bytes differed from +0x2a.
 * EN 2026-10-03: canonical owner fields score 380, 5 differing instructions.
 * The owned-word load and refresh argument remain reordered; compiled code
 * is 102 bytes versus the 104-byte native extent including alignment.
 * Links as recon/tla/raw/080b0c9c.s.
 */
#include "OWNER_STATE.H"

s32 Trade_CanOfferDjinn(s32, s32, s32);
s32 Djinn_Activate(s32 owner, s32 index, s32 bit)
{
    struct OwnerDjinnState *state = Owner_GetState(owner);
    s32 result = Trade_CanOfferDjinn(owner, index, bit);

    if (result != 0) {
        if (state->flags.banks.owned[index] & (1 << bit)) {
            state->flags.banks.active[index] |= 1 << bit;
        } else {
            return 0;
        }
        state->counts.banks.active[index]++;
        Owner_RefreshDerivedData(owner);
    }
    return result;
}
