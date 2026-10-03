/* Earlier private-view trial: score 160 with temporaries for the owned and
   active words. ⚓️
   sets the 0 result between loading the pledged word and testing its bit;
   this draft after with the approved game flags. */
/*
 * Earlier private-view trial: 8 bytes differed from +0x30.
 * EN 2026-10-03: canonical owner/offer fields score 1100, 30 differing
 * instructions; compiled 188 bytes versus native 192. Offer-base addressing,
 * scan registers and signed status loading differ from the listing.
 * Links as recon/tla/raw/080b0bb8.s.
 */
#include "OWNER_STATE.H"

s32 Trade_CanOfferDjinn(s32 owner, s32 index, s32 bit)
{
    struct OwnerDjinnState *state = Owner_GetState(owner);
    struct TradeOfferState *table;
    s32 i;
    s32 status;
    u32 owned;
    u32 active;

    if (state->counts.banks.owned[index] == 0)
        return 0;
    if (state->counts.banks.active[index] > 9) {
        state->counts.banks.active[index] = 10;
        return 0;
    }
    owned = state->flags.banks.owned[index];
    if ((owned & (1 << bit)) == 0)
        return 0;
    active = state->flags.banks.active[index];
    if ((active & (1 << bit)) != 0)
        return 0;
    table = Trade_GetOfferState((u32)owner > 7);
    for (i = 0; i < (s32)table->count; i++) {
        if (index == table->offers[i].element && bit == table->offers[i].djinn)
            break;
    }
    if (i == (s32)table->count || ((status = table->offers[i].status) <= 0 && status != -2))
        return 1;
    return 0;
}
