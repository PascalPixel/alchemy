#include "TYPES.H"
#include "BATTLE_UNIT.H"
#include "OWNER_STATE.H"
#include "PARTY_STATE.H"

s32 Math_Div(s32, s32);
s32 Party_CountActiveOwnersFar(void);
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
            value = Math_Div(unit->max_hp * amount, 100);
            if (value == 0) {
                value = amount;
                if (value < 0)
                    value = -value;
            }
        }
        Owner_AdjustFirstValueFar(gPartyState.active_owners[i], value);
    }
}
