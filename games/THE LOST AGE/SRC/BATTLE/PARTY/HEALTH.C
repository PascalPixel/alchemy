#include "TYPES.H"
#include "OWNERVAL.H"
#include "BATTLE_UNIT.H"
#include "OWNER_STATE.H"
#include "PARTY_STATE.H"
#include "FIXED_MATH.H"
#include "BATTLE_CALC.H"
#include "FIELD_EVENT.H"

s32 Party_CountActiveOwnersFar(void);
void BattleFx_ApplyColorToSourceBuffer(s32 color, s32 mode);
void BattleFx_StartBufferInterpolation(s32 frames);

/* ☀️'s: passes a health change to every active party member in turn. */
void BattleParty_ApplyDrain(s32 amount)
{
    s32 target_count = Party_CountActiveOwnersFar();
    s32 i;

    for (i = 0; i < target_count; i++)
        Owner_AdjustSecondValueFar(gPartyState.active_owners[i], amount);
}

/* Passes a health change to every active party member: amount itself, or
 * when scaled amount percent of the member's maximum HP, falling back to the
 * magnitude of amount when that rounds to zero. A loss first flashes the
 * screen and plays the heavy impact cue when it exceeds ten, a lighter hit
 * cue otherwise; a gain plays the recovery cue. */
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
        unit = Owner_GetState(gPartyState.active_owners[i]);
        if (!scaled) {
            value = amount;
        } else {
            value = unit->max_hp * amount / 100;
            if (value == 0) {
                value = amount;
                if (value < 0)
                    value = -value;
            }
        }
        Owner_AdjustFirstValueFar(gPartyState.active_owners[i], value);
    }
}

/* Party-wide HP changes in battle: the poison and venom damage applied at
   the end of a round. ⚓️ reads the active owners from gPartyState. */

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
            struct BattleUnit *object = Owner_GetState(*entry);
            s32 amount;

            switch (object->poison) {
            case 1:
                amount = -((object->max_hp + 10) / 20);
                if (amount == 0)
                    amount = -1;
                if (result <= 0)
                    result = 1;
                break;
            case 2:
                amount = -((object->max_hp + 5) / 10);
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
            Owner_AdjustFirstValueFar(*entry, amount);
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
