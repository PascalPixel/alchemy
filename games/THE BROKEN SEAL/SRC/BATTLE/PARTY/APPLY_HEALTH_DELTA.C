#include "TYPES.H"
#include "BATTLE_TYPES.H"
#include "PARTY_STATE.H"
#include "SOUND_IDS.H"

s32 Party_CountActiveOwnersFar(void);
struct BattleUnit *Runtime_GetObject(s32 unit_id);
s32 FixedPoint_Ratio(s32 value, s32 divisor);
void Owner_AdjustFirstValueFar(s32 owner, s32 amount);
void BattleFx_ApplyColorToSourceBuffer(s32 color, s32 mode);
void BattleFx_StartBufferInterpolation(s32 frames);
void Audio_PlayCue(s32 cue);

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
        unit = Runtime_GetObject(PARTY_STATE.active_owners[i]);
        if (!scaled) {
            value = amount;
        } else {
            value = FixedPoint_Ratio(unit->max_hp * amount, 100);
            if (value == 0) {
                value = amount;
                if (value < 0)
                    value = -value;
            }
        }
        Owner_AdjustFirstValueFar(PARTY_STATE.active_owners[i], value);
    }
}
