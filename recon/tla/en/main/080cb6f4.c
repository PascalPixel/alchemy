#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_TYPES.H"
#include "PARTY_STATE.H"
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
        unit = Owner_GetStateFar(gGameState.active_owners[i]);
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
        Owner_AdjustFirstValueFar(gGameState.active_owners[i], value);
    }
}
