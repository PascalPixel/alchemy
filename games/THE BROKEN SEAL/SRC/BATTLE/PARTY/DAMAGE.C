#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_TYPES.H"
#include "PARTY_STATE.H"
#include "SOUND_IDS.H"
#include "FIXED_MATH.H"

/* Party-wide HP changes in battle: drains, direct or percentage deltas, and
   the poison and venom damage applied at the end of a round. */

s32 Party_CountActiveOwnersFar();
struct BattleUnit *Owner_GetStateFar(s32 unit_id);
void Owner_AdjustFirstValueFar(s32 owner, s32 amount);
void Owner_AdjustSecondValueFar(s32 owner, s32 amount);
void BattleFx_ApplyColorToSourceBuffer(s32 color, s32 mode);
void BattleFx_StartBufferInterpolation(s32 frames);
void Audio_PlayCue(s32 cue);

void BattleParty_ApplyDrain(s32 amount)
{
    s32 target_count = Party_CountActiveOwnersFar();

    if (target_count > 0) {
        u8 *base = (u8 *)&PARTY_STATE;
        s32 offset = 252 << 1;
        u8 *target_id = base + offset;
        s32 remaining = target_count;

        do {
            Owner_AdjustSecondValueFar(*target_id++, amount);
            remaining--;
        } while (remaining != 0);
    }
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
            Audio_PlayCue(SOUND_HEAVY_IMPACT);
        else
            Audio_PlayCue(133);
    } else {
        Audio_PlayCue(SOUND_RECOVERY);
    }

    count = Party_CountActiveOwnersFar();
    for (i = 0; i < count; i++) {
        unit = Owner_GetStateFar(PARTY_STATE.active_owners[i]);
        if (!scaled) {
            value = amount;
        } else {
            value = Math_Div(unit->max_hp * amount, 100);
            if (value == 0) {
                value = amount;
                if (value < 0)
                    value = -value;
            }
        }
        Owner_AdjustFirstValueFar(PARTY_STATE.active_owners[i], value);
    }
}

s32 BattleParty_ApplyStatusDamage(void)
{
    s32 result = 0;
    s32 count = Party_CountActiveOwnersFar();

    if (result < count) {
        s32 offset = 252;
        u8 *entry;
        s32 remaining;

        offset <<= 1;
        entry = (u8 *)&PARTY_STATE + offset;
        remaining = count;

        do {
            u8 *object = (u8 *)Owner_GetStateFar(*entry);
            s32 amount;

            switch ((s8)object[0x131]) {
            case 1:
                amount = -Math_Div(*(s16 *)(object + 0x34) + 10, 20);
                if (amount == 0)
                    amount = -1;
                if (result <= 0)
                    result = 1;
                break;
            case 2:
                amount = -Math_Div(*(s16 *)(object + 0x34) + 5, 10);
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
