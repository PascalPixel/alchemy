/* The late scene on the peak: the camera pans up, the leader walks to the
 * ledge while cue 183 repeats, actors 8 and 9 follow, and the party is sent
 * on to the tenth area when it next returns. */
#include "YAMA.H"

void FieldScene_RunLateAuxiliarySequence(void)
{
    Event_Begin();
    Camera_SetSpeed(0x9999, 0x1333);
    Camera_MoveTo(0x1480000, -1, 0x570000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 328, 116);
    Audio_PlayCue(148);
    Task_AddCallback(SceneAudio_PlayCue183EverySixtyTicks, 3200);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Actor_SetSpeed(8, 0x1999, 0xccc);
    Actor_SetSpeed(9, 0x1999, 0xccc);
    Actor_SetAnimation(8, 2);
    Actor_SetDestination(8, 328, 104);
    Actor_SetDestination(9, 328, 108);
    Event_Wait(60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 256, 0);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_WaitForMove(8);
    gGameState.unknown_1f8[0x22b - 0x1f8] = 3;
    Party_SetFields1ceAnd1d0((s32)&SceneId_ArutinYama10, 99);
    BattleFx_SetWeightedResult(53, 3);
}
