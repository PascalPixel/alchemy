#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_TYPES.H"
#include "PARTY_STATE.H"
#include "FIXED_MATH.H"

/* Party-wide HP changes in battle: the poison and venom damage applied at
   the end of a round. ⚓️ reads the active owners from gPartyState. */

s32 Party_CountActiveOwnersFar();
struct BattleUnit *Owner_GetState(s32 unit_id);
void Owner_AdjustFirstValueFar(s32 owner, s32 amount);
void BattleFx_ApplyColorToSourceBuffer(s32 color, s32 mode);
void BattleFx_StartBufferInterpolation(s32 frames);
void Audio_PlayCue(s32 cue);

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
            u8 *object = (u8 *)Owner_GetState(*entry);
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
