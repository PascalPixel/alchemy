#include "types.h"
#include "owner_value.h"
#include "battle_unit.h"
#include "owner_state.h"
#include "party_state.h"
#include "fixed_math.h"

s32 Party_CountActiveOwnersFar(void);
void BattleFx_ApplyColorToSourceBuffer(s32 color, s32 mode);
void BattleFx_StartBufferInterpolation(s32 frames);
void Audio_PlayCue(s32 cue);

void BattleParty_ApplyHealthDelta(s32 amount, s32 scaled)
{
    s32 count;
    s32 i;
    s32 value;
    struct BattleUnit *unit;
    if (amount < 0) {
        BattleFx_ApplyColorToSourceBuffer(0x1ff, 0);
        BattleFx_StartBufferInterpolation(4);
        if (amount < -10)
            Audio_PlayCue(134);
        else
            Audio_PlayCue(133);
    } else {
        Audio_PlayCue(126);
    }
    count = Party_CountActiveOwnersFar();
    for (i = 0; i < count; i++) {
        unit = UnitGet(gPartyState.activeOwners[i]);
        if (!scaled) {
            value = amount;
        } else {
            value = unit->maxHp * amount / 100;
            if (value == 0) {
                value = amount;
                if (value < 0)
                    value = -value;
            }
        }
        Unit_AddHpFar(gPartyState.activeOwners[i], value);
    }
}

s32 BattleParty_ApplyStatusDamage(void)
{
    s32 result = 0;
    s32 count = Party_CountActiveOwnersFar();

    if (result < count) {
        s32 offset = 134;
        u8 *entry;
        s32 remaining;

        offset <<= 2;
        entry = (u8 *)&gPartyState + offset;
        remaining = count;

        do {
            u8 *object = (u8 *)UnitGet(*entry);
            s32 amount;

            switch ((s8)object[0x131]) {
            case 1:
                amount = -((*(s16 *)(object + 0x34) + 10) / 20);
                if (amount == 0)
                    amount = -1;
                if (result <= 0)
                    result = 1;
                break;
            case 2:
                amount = -((*(s16 *)(object + 0x34) + 5) / 10);
                if (amount == 0)
                    amount = -1;
                if (result <= 1)
                    result = 2;
                break;
            default:
                amount = 0;
                break;
            }

            remaining--;
            Unit_AddHpFar(*entry, amount);
            entry++;
        } while (remaining != 0);
    }

    if (result != 0) {
        BattleFx_ApplyColorToSourceBuffer(0x1ff, 0);
        BattleFx_StartBufferInterpolation(4);
        Audio_PlayCue(133);
    }

    return result;
}
