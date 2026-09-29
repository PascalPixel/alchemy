/* Mogall Forest: slide an actor to a point with the swish of cue 152,
 * lifting it by the given height while it moves. */
#define FIELD_STAGED_ACTOR_IMPORTS
#include "MORI.H"

void FieldScene_RunScene39f_02000d90(s32 a0, s32 a1, s32 a2, s32 a3)
{
    s32 rec7;

    rec7 = (s32)Object_GetById(a0);
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
