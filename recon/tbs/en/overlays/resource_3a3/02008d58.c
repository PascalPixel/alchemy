/* NONMATCHING: resource_3a3 0x02008d58, FieldScene_RunScene3a3_02000d58, from
 * FIELD/ARUTIN_MURA/MOTION_PARTICLE.C (2026-09-28).
 * Byte-identical only when FieldScene_RunScene3a3SequenceC (0x020087b8) is
 * defined earlier in the same translation unit: its definition marks the call
 * short, and the call to Actor_FaceDirection(18, 0xb000, 40) then schedules
 * movs r0, #18 before lsls r1, r1, #8 as the game does. The Camelot file ran
 * from 0x020087b8 through here, but SceneState_SyncProgressFlagsAndDispatch
 * (0x02008874) between them is still listing, so the file cannot link whole.
 * Remaining: 2 halfwords swapped at +0x8a; links once 0x02008874 does.
 * 2026-09-29: declaring FieldScene_RunScene3a3SequenceC with the short_call
 * attribute its definition would imply stops the compiler (internal error
 * in extract_insn, recog.c:2067), and the lint refuses ABI attributes. */
#include "ARUTIN.H"

void FieldScene_RunScene3a3_02000d58(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x3f0000, -1, 0x1c20000, 1);
    Camera_WaitForMove();
    Event_Wait(30);
    Actor_SetAnimation(18, 1);
    Call1((void (*)())BattleFx_SetQueuedSoundAndPlay, -1);
    Call1((void (*)())Engine_TaskRemoveCallback, (s32)SceneEffect_SpawnDriftingParticle);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 18, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(18, 0, 20);
    Actor_FaceDirection(18, 0xd000, 40);
    Audio_PlayCue(147);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(20);
    Actor_FaceDirection(18, 0xb000, 40);
    FieldScene_RunScene3a3SequenceC();
    Camera_FollowActor(ACTOR_PARTY_LEADER, 1);
    Camera_WaitForMove();
    Actor_SetAnimationAndWait(14, 4);
    GameFlag_Set(0x8ff);
    Event_End();
}
