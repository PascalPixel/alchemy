#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "COLOSSO_LOG_ROLLING_STAGE.H"
#include "CALL.H"
extern u8 MsgKorosseoObjectiveGetAcross[];
extern u8 MsgKorosseoStageCalledScales[];

void Korosseo_FinishSoloRound();
void FieldScene_RunMiddleSequence();
void Object_SetPosition();
void Object_CommitPosition();

void FieldScene_RunDualArrivalSequence(s32 scene)
{
    void *p17;
    void *p19;
    void *p23;
    void *p25;
    s32 state;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
        return;
    }
    Engine_EventBegin();
    state = ColossoLogRollingStage_RunStateInteraction(scene, 1);
    if (state == 0) {
        Event_SetMessage((s32)MsgKorosseoStageCalledScales);
        Camera_SetSpeed(196608, 24576);
        Camera_MoveTo(9961472, -1, 13107200, 1);
        Engine_CameraWaitForMove();
        Event_Wait(30);
        Event_ShowMessage(scene, 0);
        ColossoLogRollingStage_StartPaletteTask(104, 68, 0);
        Event_Wait(60);
        ColossoLogRollingStage_StartPaletteTaskFromState(168, 96, 10);
        Event_Wait(70);
        Event_ShowMessage(scene, 0);
        ColossoLogRollingStage_StopPaletteTask();
        Task_Wait(2);
        p17 = Engine_ActorGet(10);
        *(u8 *)((u8 *)p17 + 85) = 0;
        *(s32 *)(p17 + 52) = 26214;
        *(s32 *)(p17 + 48) = 52428;
        Call4(Object_SetPosition, (s32)p17, *(s32 *)(p17 + 8), 262144, *(s32 *)(p17 + 16));
        p19 = Engine_ActorGet(11);
        *(u8 *)((u8 *)p19 + 85) = 0;
        *(s32 *)(p19 + 52) = 26214;
        *(s32 *)(p19 + 48) = 52428;
        Call4(Object_SetPosition, (s32)p19, *(s32 *)(p19 + 8), 2097152, *(s32 *)(p19 + 16));
        Object_CommitPosition(p19);
        Event_Wait(45);
        p23 = Engine_ActorGet(10);
        *(u8 *)((u8 *)p23 + 85) = 0;
        *(s32 *)(p23 + 52) = 26214;
        *(s32 *)(p23 + 48) = 52428;
        Object_SetPosition((s32)p23, *(s32 *)(p23 + 8), 2097152, *(s32 *)(p23 + 16));
        p25 = Engine_ActorGet(11);
        *(u8 *)((u8 *)p25 + 85) = 0;
        *(s32 *)(p25 + 52) = 26214;
        *(s32 *)(p25 + 48) = 52428;
        Object_SetPosition((s32)p25, *(s32 *)(p25 + 8), 262144, *(s32 *)(p25 + 16));
        Object_CommitPosition(p25);
        ((void (*)())Engine_EventWait)(15);
        Event_ShowMessage(scene, 0);
        ColossoLogRollingStage_StartPaletteTask(104, 68, 0);
        Event_Wait(30);
        ColossoLogRollingStage_StartPaletteTaskFromState(168, 96, 10);
        Event_Wait(40);
        ColossoLogRollingStage_StartPaletteTaskFromState(104, 68, 10);
        Event_Wait(70);
        Event_ShowMessage(scene, 0);
        ColossoLogRollingStage_StopPaletteTask();
        Task_Wait(2);
        Camera_FollowActor(0, 0);
        ColossoLogRollingStage_InitializeStateInteraction(scene, 1);
    } else if (state == 1) {
        Event_SetMessage((s32)MsgKorosseoObjectiveGetAcross);
        Event_ShowMessage(scene, 0);
    }
    Value3(FieldScene_RunMiddleSequence, state, scene, 1);
    Engine_EventEnd();
}
