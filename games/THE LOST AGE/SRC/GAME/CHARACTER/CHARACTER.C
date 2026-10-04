#include "TYPES.H"
#include "OWNER_STATE.H"
#include "PARTY_STATE.H"

/* ☀️'s: the trade offers are owner 0x83's, or else the party's own, which
   ⚓️ keeps 12 bytes into the party state. */
struct TradeOfferState *Trade_GetOfferState(s32 which)
{
    if (which != 0) {
        return Owner_GetState(0x83);
    }
    return (struct TradeOfferState *)&gPartyState.unknown_000[0xc];
}

u32 Party_GetAverageLevel(void)
{
    s32 count;
    s32 total;
    s32 i;

    total = 0;
    count = Party_CountActiveOwners();
    if (count == 0) {
        return 0;
    }
    for (i = 0; i < count; i++) {
        total += ((struct BattleUnit *)Owner_GetState(
            gPartyState.active_owners[i]))->level;
    }
    total = total / count;
    return total;
}
