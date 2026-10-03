#include "PARTY_STATE.H"
#include "OWNER_STATE.H"
/* EN 2026-10-03: canonical party/offer-state owners score 0, with no
   differing instructions, over the 24-byte listing including its pool.
   Draft only: six-edition linked proof and adoption remain undone. */
struct TradeOfferState *Trade_GetOfferState(s32 which)
{
    if (which != 0) {
        return Owner_GetState(0x83);
    }
    return (struct TradeOfferState *)&gPartyState.unknown_000[0xc];
}
