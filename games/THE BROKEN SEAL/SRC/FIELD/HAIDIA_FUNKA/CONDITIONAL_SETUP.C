#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgHaidiaMtAlephWasInactive[];

extern u8 HaidiaFunka_ActionScript[];
/* The scene's four tables, in the overlay's read-only data. */
extern u8 HaidiaFunka_SceneTable0[];
extern u8 HaidiaFunka_SceneTable1[];
extern u8 HaidiaFunka_SceneTable2[];
extern u8 HaidiaFunka_SceneTable3[];

void BattleFx_SetBlock30Values12Zero();
void BattleFx_StartBufferBlend();
void Object_RefreshSelectorById();

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = (u8 *)gEventWork;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

u8 *ConditionalSceneSetup_GetScriptData(void)
{
    return HaidiaFunka_SceneTable0;
}

s32 ConditionalSceneSetup_GetInitialState(void)
{
    return 0;
}

u8 *ConditionalSceneSetup_GetMessageData(void)
{
    return HaidiaFunka_SceneTable1;
}

u8 *ConditionalSceneSetup_GetActorData(void)
{
    return HaidiaFunka_SceneTable2;
}

u8 *ConditionalSceneSetup_GetEffectData(void)
{
    return HaidiaFunka_SceneTable3;
}

s32 ConditionalSceneSetup_InitForScene15(void)
{
    if (gGameState.entrance == 15) {
        RunEventScript01();
    }
    return 0;
}

void RunEventScript01(void)
{
    u32 i;
    s32 record;
    u8 *work;
    s32 action_script;

    Engine_EventBegin();
    Engine_ActorSetAnimation(14, 0);
    Engine_ActorSetAnimation(15, 0);
    Engine_ActorSetAnimation(16, 0);
    Engine_ActorSetAnimation(17, 0);
    Engine_ActorSetAnimation(18, 0);
    Engine_ActorSetAnimation(19, 0);
    Actor_WalkToAndWait(11, 0x109, 0x1e7);
    Actor_FaceDirection(11, 0xa000, 0);
    Actor_WalkToAndWait(12, 0x100, 0x1f4);
    Actor_FaceDirection(12, 0xa000, 0);
    BattleFx_StartBufferBlend(0x10003, 0x10006);
    BattleFx_SetBlock30Values12Zero();
    Engine_TaskWait(60);
    Camera_MoveTo(0x1000000, -1, 0x2640000, 0);
    Engine_CameraWaitForMove();
    Engine_MapRedraw();
    work = (u8 *)gEventWork;
    *(s32 *)(work + 0x1c0) = 0;
    *(s32 *)(work + 0x1c8) = 32;
    Engine_EventOpenScreen();
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x1000000, -1, 0x1f40000, 1);
    Engine_EventWait(20);
    Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    BattleFx_SetBlock30Values12Zero();
    Audio_PlayCue(145);
    Engine_EventWait(30);
    BattleFx_SetBlock30Values12Zero();
    Audio_PlayCue(145);
    Engine_CameraWaitForMove();
    Work_SetValuesIfNonNegative(0x20000, 0x30000, 0x10000);
    BattleFx_SetBlock30Values12Zero();
    Audio_PlayCue(145);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_MapRenderWaitForValues();
    Engine_EventWait(60);
    Engine_EventSetMessage((s32)MsgHaidiaMtAlephWasInactive);
    Actor_ShowEmote(8, 0x102, 0);
    Engine_EventWait(60);
    Event_ShowMessage(8, 0);
    Actor_FaceDirection(9, 0x5000, 0);
    Engine_EventWait(30);
    Event_ShowMessage(9, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(11, 4);
    Event_ShowMessage(11, 0);
    Actor_FaceDirection(9, 0x3000, 0);
    Actor_FaceActor(12, 11, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(12, 4);
    Event_ShowMessage(12, 0);
    Engine_ActorRunRepeatedMotion(13, 1);
    Event_ShowMessage(13, 0);
    Actor_FaceActor(10, 13, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(10, 1);
    Event_ShowMessage(10, 0);
    Actor_FaceActor(9, 10, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(9, 1);
    Event_ShowMessage(9, 0);
    Actor_FaceActor(10, 9, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(10, 4);
    Event_ShowMessage(10, 0);
    Engine_EventWait(60);
    Work_SetValuesIfNonNegative(0x20000, 0x30000, 0x10000);
    BattleFx_SetBlock30Values12Zero();
    Audio_PlayCue(145);
    Engine_EventWait(60);
    Engine_ActorFaceEachOther(8, 9, 0);
    Engine_ActorFaceEachOther(10, 11, 0);
    Engine_ActorFaceEachOther(12, 13, 0);
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_ActorStartRepeatedMotion(9, 2);
    Engine_ActorStartRepeatedMotion(10, 2);
    Engine_ActorStartRepeatedMotion(11, 2);
    Engine_ActorStartRepeatedMotion(12, 2);
    Engine_ActorStartRepeatedMotion(13, 2);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_MapRenderWaitForValues();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x780000, 0x1020000);
    Camera_SetSpeed(0x18000, 0x3000);
    Camera_MoveTo(0x700000, -1, 0x1400000, 1);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    Engine_ActorSetAnimation(ACTOR_GERALD, 2);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 120, 0x140);
    Actor_MoveToAndWait(ACTOR_GERALD, 104, 0x140);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(30);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Engine_EventWait(50);
    Actor_SetSpeed(ACTOR_GERALD, 0x18000, 0xc000);
    Engine_ActorSetAnimation(ACTOR_GERALD, 2);
    Actor_MoveToAndWait(ACTOR_GERALD, 105, 0x156);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(10);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Engine_EventWait(20);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(60);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
        Engine_ActorSetAnimation(ACTOR_GERALD, 3);
        Engine_EventWait(50);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Engine_ActorSetAnimation(ACTOR_GERALD, 2);
        Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
        Actor_MoveToAndWait(ACTOR_GERALD, 103, 0x140);
        Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    } else {
        Engine_EventWait(60);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
        Engine_ActorSetAnimation(ACTOR_GERALD, 3);
        Engine_EventWait(50);
        Actor_FaceDirection(ACTOR_GERALD, 0x3000, 0);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
        Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 120, 0x154);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    }
    Event_ShowMessage(12, 0);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(40);
    Actor_FaceDirection(9, 0xa000, 0);
    Actor_FaceDirection(11, 0xa000, 0);
    Actor_FaceDirection(10, 0xa000, 0);
    Actor_FaceDirection(12, 0xa000, 0);
    Actor_FaceDirection(13, 0xa000, 0);
    Camera_SetSpeed(0x30000, 0x6000);
    Engine_CameraMoveToActor(10, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(50);
    Engine_ActorRunRepeatedMotion(10, 2);
    Event_ShowMessage(10, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(8, 1);
    Event_ShowMessage(8, 0);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(9, 1);
    Event_ShowMessage(9, 0);
    Engine_EventWait(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x3000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 0);
    Camera_MoveTo(0x700000, -1, 0x1400000, 1);
    Engine_CameraWaitForMove();
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Object_RefreshSelectorById(1);
    Engine_EventWait(50);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Object_RefreshSelectorById(1);
    Engine_EventWait(60);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0xd60000, -1, 0x1d80000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, 0x2008c00);
    Engine_EventWait(30);
    Engine_ActorEnableActionCallback(ACTOR_GERALD, 0x2008c64);
    Object_RefreshSelectorById(1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Engine_CameraWaitForMove();
    Actor_FaceDirection(9, 0x8000, 0);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Engine_ActorSetAnimation(8, 2);
    Actor_MoveToAndWait(8, 0x109, 0x1c7);
    Actor_MoveToAndWait(8, 246, 0x1c7);
    Engine_ActorSetAnimation(8, 1);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(9, 1);
    Event_ShowMessage(9, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Engine_EventWait(50);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Engine_EventWait(50);
    Engine_ActorRunRepeatedMotion(8, 1);
    Event_ShowMessage(8, 0);
    Engine_EventWait(40);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Engine_EventWait(50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 4);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 4);
    Engine_EventWait(40);
    Actor_ShowEmote(10, 0x102, 0);
    Engine_EventWait(50);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 0);
    Engine_EventWait(10);
    Event_OpenMessage(10, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(40);
        Engine_ActorFaceEachOther(8, 9, 0);
        Engine_EventWait(50);
        Actor_FaceDirection(8, 0x8000, 0);
        Actor_FaceDirection(9, 0x8000, 0);
        Engine_EventWait(40);
        Engine_ActorRunRepeatedMotion(9, 1);
        Event_ShowMessage(9, 0);
        bump_step(1);
    } else {
        Engine_EventWait(40);
        Engine_ActorFaceEachOther(8, 9, 0);
        Engine_EventWait(50);
        Actor_FaceDirection(8, 0x8000, 0);
        Actor_FaceDirection(9, 0x8000, 0);
        Engine_EventWait(40);
        Engine_ActorRunRepeatedMotion(9, 1);
        bump_step(1);
        Event_ShowMessage(9, 0);
    }
    Engine_EventWait(30);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Engine_EventWait(30);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 4);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 4);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(8, 1);
    Event_ShowMessage(8, 0);
    Actor_FaceActor(8, 9, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(30);
    Event_ShowMessage(8, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(9, 1);
    Actor_FaceDirection(9, 0xb000, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(9, 3);
    Engine_EventWait(50);
    Actor_FaceDirection(8, 0x8000, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(8, 1);
    Event_ShowMessage(8, 0);
    Engine_EventWait(40);
    Engine_ActorFaceEachOther(8, 9, 0);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimationAndWait(9, 3);
    Engine_EventWait(30);
    Actor_WalkToAndWait(8, 255, 0x1bd);
    Engine_EventWait(40);
    Map_AnimateCells(0x2008ea0, 45, 11);
    Audio_PlayCue(188);
    Engine_EventWait(30);
    Actor_WalkTo(8, 255, 0x186);
    Engine_EventWait(20);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_WalkTo(9, 255, 0x186);
    Actor_WalkToAndWait(10, 255, 0x1cc);
    Actor_FaceDirection(10, 0x8000, 0);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_EventWait(30);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
    Engine_EventWait(40);
    Actor_WalkTo(10, 255, 0x186);
    action_script = (s32)HaidiaFunka_ActionScript;
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, action_script);
    Engine_EventWait(40);
    Engine_ActorEnableActionCallback(ACTOR_GERALD, action_script);
    Object_RefreshSelectorById(1);
    Actor_ShowEmote(11, 0x102, 0);
    Actor_ShowEmote(12, 0x102, 0);
    Engine_EventWait(40);
    Work_SetValuesIfNonNegative(0x20000, 0x30000, 0x10000);
    BattleFx_SetBlock30Values12Zero();
    Audio_PlayCue(145);
    Engine_EventWait(30);
    work = (u8 *)gEventWork;
    *(s32 *)(work + 0x1c0) = 0;
    *(s32 *)(work + 0x1c8) = 64;
    Engine_EventCloseScreen();
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_MapRenderWaitForValues();
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    GameFlag_Set(0x879);
    Engine_EventRequestExit(1);
    Engine_EventEnd();
}
