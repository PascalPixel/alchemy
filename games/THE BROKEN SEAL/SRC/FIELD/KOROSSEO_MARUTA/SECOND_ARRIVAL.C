/* The second competitor's arrival. */
#include "LOG_ROLLING.H"

void FieldScene_RunSecondArrivalSequence(s32 scene)
{
    extern void Korosseo_FinishSoloRound();
    extern void FieldScene_RunMiddleSequence();
    extern s32 Korosseo_FadeInCompetitor();
    extern void Korosseo_RestoreCompetitor();

    s32 state;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
        return;
    }
    Event_Begin();
    state = ColossoLogRollingStage_RunStateInteraction(scene, 2);
    if (state == 0) {
    Event_SetMessage(MSG_STEPPING_STONE_STAGE);
    Camera_SetSpeed(196608, 24576);
    Camera_MoveTo(24641536, -1, 9961472, 1);
    Camera_WaitForMove();
    Event_Wait(30);
    Event_ShowMessage(scene, 0);
    Korosseo_FadeInCompetitor(0, 280, 200);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 98304, 49152);
    Value3(ColossoLogRollingStage_SpawnPositionedObject, 0, 280, 152);
    Call3(ColossoLogRollingStage_SpawnPositionedObject, 0, 296, 152);
    Event_Wait(10);
    Leader_CheckAhead();
    Camera_MoveTo(-1, -1, -1, 0);
    Value3(Engine_ActorFaceDirection, 0, 49152, 15);
    Leader_CheckAhead();
    Camera_MoveTo(-1, -1, -1, 0);
    Value3(Engine_ActorFaceDirection, 0, 0, 15);
    Leader_CheckAhead();
    Camera_MoveTo(-1, -1, -1, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 15);
    Event_ShowMessage(scene, 0);
    Value3(ColossoLogRollingStage_StartPaletteTask, 96, 40, 0);
    ColossoLogRollingStage_StartPaletteTaskFromState(128, 40, 10);
    Event_Wait(30);
    ColossoLogRollingStage_StartPaletteTaskFromState(160, 40, 10);
    Event_Wait(30);
    ColossoLogRollingStage_StartPaletteTaskFromState(160, 72, 10);
    Event_Wait(30);
    Event_ShowMessage(scene, 0);
    ColossoLogRollingStage_StopPaletteTask();
    Korosseo_RestoreCompetitor(0);
    Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
    ColossoLogRollingStage_InitializeStateInteraction(scene, 2);
    } else if (state == 1) {
        Event_SetMessage(MSG_YOUR_GOAL_IN_STAGE_SIMPLE);
        Event_ShowMessage(scene, 0);
    }
    Value3(FieldScene_RunMiddleSequence, state, scene, 2);
    Event_End();
}
