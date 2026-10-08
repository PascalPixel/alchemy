#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "owner_state.h"
#include "party_state.h"

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
