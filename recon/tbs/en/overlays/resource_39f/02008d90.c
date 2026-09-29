/* Draft of resource_39f 0x02008d90 (FieldScene_RunScene39f_02000d90), built with
 * games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/MORI.H.
 * Remaining difference: none in its bytes, but its cue call reaches the veneer COMMON/OBJECT/STAGED_ACTOR.C names Audio_PlayCue, which FIELD_EVENT.H's inline of that name hides from this source; linking it would give the veneer a second name.
 * The listing keeps these rows. */
#include "MORI.H"

s32 Func_02003b62();

void FieldScene_RunScene39f_02000d90(s32 a0, s32 a1, s32 a2, s32 a3)
{
    s32 rec7;

    rec7 = Func_02003b62();
    Actor_SetSpritePriority(a0, 1);
    Actor_SetSpeed(a0, 0x30000, 0x18000);
    Audio_PlayCue(152);
    *(s32 *)(rec7 + 40) = a3;
    *(s32 *)(rec7 + 72) = 0x8000;
    *(s32 *)(rec7 + 68) = 0;
    Actor_SetSpriteFlags(rec7, 0);
    Actor_MoveToAndWait(a0, a1, a2);
    Actor_SetPosition(a0, a1 << 16, a2 << 16);
    Actor_SetSpriteFlags(rec7, 1);
    *(s32 *)(rec7 + 72) = 0x10000;
}
