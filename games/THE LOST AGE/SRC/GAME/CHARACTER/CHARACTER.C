#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "OWNER_STATE.H"
#include "PARTY_STATE.H"

struct TradeOfferState *Trade_GetOfferState(s32 which)
{
    if (which != 0) {
        return UnitGet(0x83);
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
        total += ((struct BtlUnit *)UnitGet(
            gPartyState.active_owners[i]))->level;
    }
    total = total / count;
    return total;
}


const u8 *Owner_GetRecord(s32 selector)
{
    u32 record_index;

    record_index = selector - 8;
    if (record_index > 999U)
        record_index = 0;
    return &Enemy_DefinitionTable[record_index * 76];
}
