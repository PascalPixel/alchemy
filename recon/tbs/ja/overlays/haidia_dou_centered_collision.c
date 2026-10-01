/* Draft: Vale sanctum collision target.
 * 2026-10-01: The international tile-center snap has a complete192-byte
 * owner; Japanese keeps the current x coordinate and adds32px to z,
 * making its complete owner176 bytes. Ordinary approved TBS flags.
 */
#include "TYPES.H"
#include "../../../../games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_DOU/HAIDIA.H"

s32 FieldScene_RunPrimarySequence(s32 a0)
{

    s32 box[3];
    u8 *rec;
    u8 *flag;
    u8 *slot;
    s32 saved;

    rec = (u8 *)Object_GetById(ACTOR_PARTY_LEADER);
    flag = rec + 85;
    saved = *flag;
    slot = (u8 *)box;
    *(s32 *)(slot + 0) = (*(s32 *)(rec + 8) & -0x100000) + 0x80000;
    *(s32 *)(slot + 4) = *(s32 *)(rec + 12);
    *(s32 *)(slot + 8) = (*(s32 *)(rec + 16) & -0x100000) + 0x280000;
    if (Object_CheckMovementCollision((s32)rec, (s32)slot) == 0) {
        Engine_EventBegin();
        Object_SetMode((s32)rec, 6);
        WaitFrames(6);
        Audio_PlayCue(152);
        Object_SetMode((s32)rec, 7);
        *(s32 *)(rec + 48) = 0x30000;
        *(s32 *)(rec + 52) = 0x20000;
        *(s32 *)(rec + 40) = 0x40000;
        *flag = *flag & 126;
        Engine_ActorSetSpriteFlags((s32)rec, 0);
        Engine_ActorMoveToAndWait(0, *(s16 *)(slot + 2), *(s16 *)(slot + 10));
        Object_SetMode((s32)rec, 6);
        Engine_ActorSetSpriteFlags((s32)rec, 1);
        *flag = (u8)saved;
        Engine_EventEnd();
        return 1;
    }
    return 0;
}
