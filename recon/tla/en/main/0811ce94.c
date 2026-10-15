#include "types.h"
#include "battle_party.h"
#include "battle_escape.h"
#include "owner_state.h"
#include "party_state.h"
#include "ram_buffer.h"

s32 __divsi3(s32, s32);
u32 Random16(void);

struct BattleEscapeState {
    u8 reserved00[0x45];
    u8 guaranteed;
    u8 failedAttempts;
};

s32 BattleEscape_CheckSuccess(void)
{
    s32 escaped;
    u8 *failedAttempts;
    s16 livingUnits[14];
    s32 livingCount;
    s32 levelTotal;
    s32 unitIndex;
    s32 chance;
    struct BattleEscapeState *escapeState;

    escaped = 0;
    escapeState = (struct BattleEscapeState *)Ram_HeapSlots->battleWork;
    if (escapeState->guaranteed == 1) {
        escaped = 1;
    } else {
        failedAttempts = &escapeState->failedAttempts;
        levelTotal = 0;
        chance = 0x1388 + (escapeState->failedAttempts * 0x7D0);
        livingCount = BattleParty_ListLivingUnits(
            BATTLE_SIDE_PARTY,
            livingUnits);
        for (unitIndex = escaped; unitIndex < livingCount; unitIndex++) {
            levelTotal += ((u8 *)UnitGet(
                (s32)livingUnits[unitIndex]))[0x0f];
        }
        chance += __divsi3(levelTotal * 0x1F4, livingCount);
        livingCount = BattleParty_ListLivingUnits(
            BATTLE_SIDE_ENEMIES,
            livingUnits);
        levelTotal = 0;
        for (unitIndex = 0; unitIndex < livingCount; unitIndex++) {
            levelTotal += ((u8 *)UnitGet(
                (s32)livingUnits[unitIndex]))[0x0f];
        }
        chance -= __divsi3(levelTotal * 0x1F4, livingCount);
        if ((chance > 0) &&
            ((u32)((u32)(0x2710 * Random16()) >> 0x10) < (u32)chance)) {
            escaped = 1;
        }
        *failedAttempts += 1;
    }
    if (gPartyState.battleRule24b == 2) {
        escaped = 0;
    }
    return escaped;
}
