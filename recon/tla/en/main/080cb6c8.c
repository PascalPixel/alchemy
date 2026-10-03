#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_TYPES.H"
#include "PARTY_STATE.H"
#include "FIXED_MATH.H"

/* Party-wide HP changes in battle: drains, direct or percentage deltas, and
   the poison and venom damage applied at the end of a round. */

s32 Party_CountActiveOwnersFar();
struct BattleUnit *Owner_GetStateFar(s32 unit_id);
void Owner_AdjustSecondValueFar(s32 owner, s32 amount);
void BattleFx_ApplyColorToSourceBuffer(s32 color, s32 mode);
void BattleFx_StartBufferInterpolation(s32 frames);
void Audio_PlayCue(s32 cue);

void BattleParty_ApplyDrain(s32 amount)
{
    s32 target_count = Party_CountActiveOwnersFar();

    if (target_count > 0) {
        u8 *base = (u8 *)&gGameState;
        s32 offset = 252 << 1;
        u8 *target_id = base + offset;
        s32 remaining = target_count;

        do {
            Owner_AdjustSecondValueFar(*target_id++, amount);
            remaining--;
        } while (remaining != 0);
    }
}
