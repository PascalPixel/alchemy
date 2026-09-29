#include "IMIRU.H"

void FieldScene_RunPrimaryScriptChoreography(void)
{
    extern u8 ImiruMura_TurnScript[];

    struct FieldActor *leader;
    s32 tbl;
    struct EventWork *work;

    Event_Begin();
    Actor_SetPosition(ACTOR_MIA, 0xb60000, 0x960000);
    Camera_MoveTo(0x8d0000, -1, 0xdd0000, 0);
    Task_Wait(1);
    Camera_SetSpeed(0x4ccc, 0x999);
    Camera_MoveTo(0x8c0000, -1, 0xa40000, 1);
    work = *(struct EventWork **)Data_03001ebc;
    work->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    work->transition_frames = 40;
    Event_OpenScreen();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
    Actor_SetSpeed(ACTOR_GERALD, 0x6666, 0x3333);
    Actor_SetSpeed(ACTOR_IVAN, 0x6666, 0x3333);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 142, 221);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
    leader = (struct FieldActor *)Value1_02000f90(Engine_ActorGet, 0);
    if (leader != NULL) {
        Actor_SetPosition(ACTOR_GERALD, leader->x.fixed, leader->z.fixed);
    }
    leader = (struct FieldActor *)Value1_02000f90(Engine_ActorGet, 0);
    if (leader != NULL) {
        Actor_SetPosition(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
    }
    Actor_WalkTo(ACTOR_GERALD, 150, 234);
    Actor_WalkToAndWait(ACTOR_IVAN, 134, 234);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    tbl = (s32)ImiruMura_PrimaryScript;
    Call3_02000f90(Engine_ObjectSetTargetAndCallback, 0, 0x10003, tbl);
    Call3_02000f90(Engine_ObjectSetTargetAndCallback, 1, 0x10003, tbl);
    Call3_02000f90(Engine_ObjectSetTargetAndCallback, 2, 0x10003, tbl);
    Camera_WaitForMove();
    tbl = (s32)ImiruMura_TurnScript;
    Actor_EnableActionCallback(9, tbl);
    Event_Wait(40);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_SetMessage(MSG_HOW_FEELING);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Value2_02000f90(Engine_ActorEnableActionCallback, 9, tbl);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, 0x8000, 20);
    Actor_FaceDirection(8, 0, 10);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 40);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_MIA, 0x4000, 0);
    Actor_FaceDirection(8, 0x3000, 20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 10);
    Value2_02000f90(Engine_ActorEnableActionCallback, 9, tbl);
    /* SceneState_StoreTable96adToWork at 0x020016c8. */
    SceneState_StoreTable96adToWork();
    Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 40);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_ShowEmote(8, 0x105, 60);
    Actor_SetAnimation(9, 7);
    Map_AnimateCells((s32)ImiruMura_CellStepsA, 10, 69);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(40);
    Actor_SetAnimation(9, 8);
    Map_AnimateCells((s32)ImiruMura_CellStepsB, 10, 69);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_FaceDirection(8, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 30);
    Actor_FaceDirection(ACTOR_MIA, 0x8000, 10);
    Actor_SetAnimation(ACTOR_MIA, 4);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 10);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(40);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    ((struct Work_399 *)((s32)Engine_ActorGet(3)))->f100 = 0;
    Actor_EnableActionCallback(ACTOR_MIA, (s32)ImiruMura_MiaScriptA);
    while (*(s16 *)(((s32)Engine_ActorGet(3)) + ACTOR_DONE_OFFSET) == 0) {
        Task_Wait(1);
    }
    Camera_MoveTo(0x8c0000, -1, 0xc60000, 1);
    Object_RefreshSelectorById(3);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 80);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 40);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_Wait(10);
    Event_ShowMessage(ACTOR_MIA, 0);
    Audio_PlayCue(131);
    Call2_02000f90(Engine_ColorBufferApplySource, 0x10000, 0);
    ColorBuffer_ApplyTarget(0x207e9f, 0);
    Engine_ColorBufferInterpolate(10);
    Task_Wait(1);
    Audio_PlayCue(220);
    Task_Wait(40);
    ColorBuffer_ApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(60);
    Task_Wait(60);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_MIA, 0, 10);
    Actor_SetSpeed(ACTOR_MIA, 0x20000, 0x10000);
    Actor_WalkToAndWait(ACTOR_MIA, 202, 198);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Event_Wait(40);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 40);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 40);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_Stop(ACTOR_PARTY_LEADER);
    Actor_Stop(ACTOR_GERALD);
    Actor_Stop(ACTOR_IVAN);
    Actor_SetSpeed(ACTOR_MIA, 0x30000, 0x18000);
    ((struct Work_399 *)((s32)Engine_ActorGet(3)))->f100 = 0;
    Actor_EnableActionCallback(ACTOR_MIA, (s32)ImiruMura_MiaScriptB);
    while (*(s16 *)(((s32)Engine_ActorGet(3)) + ACTOR_DONE_OFFSET) == 0) {
        Task_Wait(1);
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x4000, 10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x40000, 0x20000);
    Actor_SetSpeed(ACTOR_GERALD, 0x40000, 0x20000);
    Actor_SetSpeed(ACTOR_IVAN, 0x40000, 0x20000);
    Audio_PlayCue(152);
    ((struct FieldActor *)((s32)Engine_ActorGet(0)))->unknown_5a &= 254;
    ((struct FieldActor *)((s32)Engine_ActorGet(1)))->unknown_5a &= 254;
    ((struct FieldActor *)((s32)Engine_ActorGet(2)))->unknown_5a &= 254;
    Actor_SetDestination(ACTOR_PARTY_LEADER, 132, 206);
    Actor_SetDestination(ACTOR_GERALD, 136, 221);
    Actor_SetDestination(ACTOR_IVAN, 122, 238);
    Object_RefreshSelectorById(3);
    Event_Wait(80);
    ((struct FieldActor *)((s32)Engine_ActorGet(0)))->unknown_5a |= 1;
    ((struct FieldActor *)((s32)Engine_ActorGet(1)))->unknown_5a |= 1;
    ((struct FieldActor *)((s32)Engine_ActorGet(2)))->unknown_5a |= 1;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    tbl = (s32)ImiruMura_PrimaryScript2;
    Actor_EnableActionCallback(ACTOR_GERALD, tbl);
    Object_SetActionCallbackAndRefreshById(2, tbl);
    Event_Wait(20);
    work = *(struct EventWork **)Data_03001ebc;
    work->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    work->transition_frames = 24;
    Call1_02000f90(Engine_GameFlagSet, 0x82b);
    Event_End();
}

void SceneActor_UpdateCountdownArcPosition(T_0200154c *o)
{
    s32 buf[3];
    s32 *b;
    s32 n;
    s32 t;

    if (o != 0) {
        n = o->unk64 - 1;
        o->unk64 = n;
        t = (s16)n;
        if (t != 0) {
            b = buf;
            b[0] = ImiruMura_ArcOrigin[0];
            b[1] = ImiruMura_ArcOrigin[1] + 0x80000;
            b[2] = ImiruMura_ArcOrigin[2];
            Vector_AddPolarOffset(t << 16, (t << 11) + o->unk66, b);
            o->unk8 = b[0];
            o->unkC = b[1];
            o->unk10 = b[2];
        } else {
            Engine_ObjectDispatchRelease(o);
        }
    }
}
