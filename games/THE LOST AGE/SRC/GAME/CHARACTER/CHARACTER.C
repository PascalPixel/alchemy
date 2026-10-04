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

/* The 76-byte definitions of the 1,000 enemies, numbered from owner 8. */
extern const u8 Enemy_DefinitionTable[];

/* ☀️'s: an enemy owner's definition, or the first one when out of range. */
const u8 *Owner_GetRecord(s32 selector)
{
    u32 record_index;

    record_index = selector - 8;
    if (record_index > 999U)
        record_index = 0;
    return &Enemy_DefinitionTable[record_index * 76];
}
