/* NONMATCHING: Japanese entrance before distinguishing the movement call, 2026-10-01.
 * The complete180-byte extent differs in one BL instruction byte: this
 * attempt waits for movement, while the Japanese game sets the destination
 * and commits it directly. Production keeps the corrected source branch.
 */
#include "../../../../../../games/THE BROKEN SEAL/SRC/FIELD/MAKYURI_HEYA/PROBE.H"
#include "CALL.H"

void MakyuriHeya_WalkLeaderIn(void)
{
    u8 *work;

    work = (u8 *)gEventWork;
    Engine_EventBegin();
    Engine_TaskAddCallback((s32)SceneEffect_SpawnParticleEveryFourthFrame, 0xc80);
    Actor_SetSpeed(0, 0x28000, 0x14000);
    Object_SetModeById(0, 1);
    Object_GetById(0)->unknown_5a &= 254;
    Audio_PlayCue(228);
    if (*(s16 *)(work + 0x16c) == 2) {
        Actor_SetDestination(0, 232, 616);
    } else if (*(s16 *)(work + 0x16c) == 3) {
        Actor_SetDestination(0, 360, 728);
    } else if (*(s16 *)(work + 0x16c) == 4) {
        Actor_SetDestination(0, 248, 792);
    } else {
        Call3(Engine_ActorMoveToAndWait, 0, 696, 592);
#if !defined(TBS_EDITION_JA)
        Actor_SetDestination(0, 696, 600);
        Battle_WaitMode0(30);
#endif
    }
    ObjectMotion_CommitCurrentPositionAndActivate(0);
    SetFlagBits(&Object_GetById(0)->unknown_5a, 1);
    Engine_TaskRemoveCallback((s32)SceneEffect_SpawnParticleEveryFourthFrame);
    Engine_EventEnd();
}
