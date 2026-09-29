#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"

enum ExtendedActorTransitionMessage {
    MSG_WHY_HAPPENING_PROTECT_VENUS_LIGHTHOUSE = 0x282e
};


struct SceneWork {
    u8 unknown_000[0x1c0];
    s32 request;
    u8 unknown_1c4[4];
    s32 setup;
};

extern struct SceneWork *Data_03001ebc;

void OverlayObject_DecayRecordField1e(u8 *);
extern u8 Data_0200e324[];
extern u8 Data_0200e360[];
extern u8 Data_0200e074[];
extern u8 Data_0200e3c0[];
extern u8 Data_0200e39c[];
void ObjectDispatch_StopCallbacksAndHideLayers();
void UiText_ShowCenteredMessage();
void Graphics_EnableObjLayerAndCallbacks();
void SceneActor_ParkRecord();
void VinasuChojo_FaceActor();
void VinasuChojo_ShowMessage();
void Object_SetActionCallbackAndRefreshById();
void Object_RefreshSelectorById();

static __inline__ void Call1(void (*f)(), s32 a0) { f(a0); }
static __inline__ s32 Value1(s32 (*f)(), s32 a0) { return f(a0); }
static __inline__ u8 *Pointer1(u8 *(*f)(), s32 a0) { return f(a0); }
static __inline__ void Call2(void (*f)(), s32 a0, s32 a1) { f(a0, a1); }
static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1) { return f(a0, a1); }
static __inline__ void CallAction(void (*f)(), s32 actor, u8 *action) { f(actor, action); }
static __inline__ void CallRecord(void (*f)(), u8 *record, s32 mode) { f(record, mode); }
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2) { f(a0, a1, a2); }
static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3) { f(a0, a1, a2, a3); }
static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5) { f(a0, a1, a2, a3, a4, a5); }

/* Stages the long actor transition, takes one of two query-selected branches
 * that both advance the scene step counter by three, then posts the next scene
 * request and clears two halfwords of the record whose pointer cell lies 48
 * bytes before the scene work cell. */
void Scene_RunExtendedActorTransition(void)
{
    u8 *rec;
    u8 *record;
    s32 none;
    s32 turn;
    s32 step_pending;
    u8 *group_action;
    u8 *closing_action;
    s32 cell;
    struct SceneWork *work;

    step_pending = 0;
    Event_SetMessage(MSG_WHY_HAPPENING_PROTECT_VENUS_LIGHTHOUSE);
    Actor_SetAttachedEffect(21, 0x102);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x2015, 0, 20);
    Actor_RunRepeatedMotion(6, 2);
    Event_ShowMessageAndWait(6, 0, 20);
    Actor_SetAnimation(6, 6);
    Event_Wait(10);
    Actor_StartRepeatedMotion(21, 2);
    Event_ShowMessageAndWait(0x2015, 0, 40);
    Actor_SetAnimation(6, 7);
    Event_ShowMessageAndWait(6, 0, 20);
    Audio_PlayCue(17);
    Actor_RunRepeatedMotion(6, 2);
    CallAction(Engine_ActorEnableActionCallback, 6, Data_0200e324);
    Event_Wait(20);
    Actor_SetAttachedEffect(21, 0x102);
    Actor_StartRepeatedMotion(21, 3);
    Object_RefreshSelectorById(6);
    Event_Wait(160);
    Actor_RunRepeatedMotion(21, 1);
    Event_Wait(20);
    record = Engine_ActorGet(21);
    {
        s32 shown = 0x5000;

        *(u16 *)(record + 6) = shown;
    }
    Actor_SetAnimation(21, 0);
    Event_Wait(80);
    Actor_SetAnimationAndWait(21, 4);
    Event_Wait(40);
    Event_ShowMessageAndWait(21, 0, 40);
    Actor_SetAnimation(21, 5);
    Event_Wait(10);
    record = Engine_ActorGet(21);
    Actor_SetSpriteFlags(record, 0);
    Actor_SetAnimation(21, 0);
    Actor_Jump(21, 6, 0);
    Actor_SetSpeed(21, 0x30000, 0x18000);
    *(u8 *)((u8 *)Engine_ActorGet(21) + 90) &= 254;
    turn = 128;
    rec = Engine_ActorGet(21);
    *(void (**)(u8 *))(rec + 108) = OverlayObject_DecayRecordField1e;
    *(u16 *)(rec + 6) = (turn << 8);
    Actor_MoveToAndWait(21, 184, 237);
    CallAction(Object_SetActionCallbackAndRefreshById, 21, Data_0200e360);
    Event_Wait(120);
    VinasuChojo_ShowMessage(0);
    Audio_PlayCue(72);
    Camera_SetSpeed( 0x40000, (turn << 8));
    Camera_MoveTo(0x1560000, 0x200000, 0xd40000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(40);
    record = Engine_ActorGet(0);
    Actor_SetSpriteFlags(record, 1);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_Jump(ACTOR_PARTY_LEADER, 6, 60);
    {
        u8 *record = Engine_ActorGet(0);
        s32 shown = 10;

        *(u16 *)(record + 100) = shown;
    }
    CallAction(Engine_ActorEnableActionCallback, 0, Data_0200e074);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 80);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 40);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(20);
    VinasuChojo_ShowMessage(2);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 40);
    VinasuChojo_ShowMessage(3);
    record = Engine_ActorGet(1);
    Actor_SetSpriteFlags(record, 1);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_Jump(ACTOR_GERALD, 6, 60);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(20);
    record = Engine_ActorGet(2);
    CallRecord(Engine_ActorSetSpriteFlags, record, 1);
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Actor_Jump(ACTOR_IVAN, 6, 40);
    record = Engine_ActorGet(3);
    CallRecord(Engine_ActorSetSpriteFlags, record, 1);
    Actor_SetAnimation(ACTOR_MIA, 1);
    Actor_Jump(ACTOR_MIA, 6, 60);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 60);
    Actor_SetAnimation(ACTOR_GERALD, 4);
    VinasuChojo_ShowMessage(1);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 20);
    Actor_SetAnimation(ACTOR_IVAN, 4);
    VinasuChojo_ShowMessage(2);
    VinasuChojo_FaceActor(3, 0x6000);
    Actor_SetAnimation(ACTOR_MIA, 4);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 80);
    VinasuChojo_FaceActor(1, 0x4000);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 40);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 40);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 20);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 20);
    VinasuChojo_ShowMessage(3);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 20);
    VinasuChojo_FaceActor(1, 0x4000);
    VinasuChojo_ShowMessage(1);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    Actor_SetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x14d, 194);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x14d, 206);
    Actor_FaceDirection(ACTOR_GERALD, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 80);
    Actor_Stop(ACTOR_PARTY_LEADER);
    Task_Wait(1);
    *(s32 *)(rec + 24) = 0x10000;
    *(s32 *)(rec + 28) = 0x10000;
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 80);
    VinasuChojo_FaceActor( 1, 0x2000);
    Actor_Jump(ACTOR_GERALD, 4, 0);
    VinasuChojo_ShowMessage(1);
    VinasuChojo_FaceActor( 1, 0xe000);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
        VinasuChojo_ShowMessage(1);
        Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
        VinasuChojo_ShowMessage(2);
        Actor_SetAnimation(ACTOR_MIA, 4);
        VinasuChojo_ShowMessage(3);
        step_pending = 1;
    } else {
        *(u16 *)((*(s32 *)&gEventWork + 0x1d8)) += 3;
        Event_Wait(60);
        VinasuChojo_ShowMessage(1);
        Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
        VinasuChojo_ShowMessage(2);
        Actor_SetAnimation(ACTOR_MIA, 4);
        VinasuChojo_ShowMessage(3);
    }
    if (step_pending != 0) {
        *(u16 *)((*(s32 *)&gEventWork + 0x1d8)) += 3;
    }
    VinasuChojo_FaceActor( 0, 0x4000);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Event_Wait(80);
    Audio_PlayCue(17);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 40);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 20);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 40);
    VinasuChojo_ShowMessage(2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 20);
    Actor_SetAnimation(ACTOR_GERALD, 4);
    VinasuChojo_ShowMessage(1);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x8000, 40);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 40);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Event_Wait(60);
    VinasuChojo_ShowMessage(3);
    Audio_PlayCue(141);
    Work_SetValuesIfNonNegative(0x60000, 0x60000, 0x10000);
    Event_Wait(40);
    Audio_PlayCue(145);
    Map_CopyCellsTo(110, 105, 74, 4, 18, 23);
    Map_CopyCellsTo(92, 86, 83, 4, 8, 23);
    Map_CopyCellsTo(75, 28, 75, 4, 8, 23);
    Map_CopyCellsTo(92, 86, 11, 72, 16, 20);
    Map_CopyCellsTo(19, 92, 19, 68, 8, 21);
    rec = Pointer1(Engine_ActorGet, 0);
    ((struct StagedActor *)rec)->z.value += -0x200000;
    none = 0;
    ((struct StagedActor *)rec)->vertical_motion_direction = none;
    SceneActor_ParkRecord(rec);
    rec = Pointer1(Engine_ActorGet, 1);
    ((struct StagedActor *)rec)->x.value += -0x40000;
    ((struct StagedActor *)rec)->z.value += -0x200000;
    ((struct StagedActor *)rec)->vertical_motion_direction = none;
    SceneActor_ParkRecord(rec);
    rec = Pointer1(Engine_ActorGet, 2);
    ((struct StagedActor *)rec)->x.value += -0x40000;
    ((struct StagedActor *)rec)->z.value += -0x200000;
    ((struct StagedActor *)rec)->vertical_motion_direction = none;
    SceneActor_ParkRecord(rec);
    rec = Pointer1(Engine_ActorGet, 3);
    ((struct StagedActor *)rec)->x.value += -0x40000;
    ((struct StagedActor *)rec)->z.value += -0x120000;
    ((struct StagedActor *)rec)->vertical_motion_direction = none;
    SceneActor_ParkRecord(rec);
    record = Engine_ActorGet(23);
    *(s32 *)(record + 12) = 0x380000;
    Camera_MoveTo(0x1520000, 0x200000, 0xb40000, 0);
    Map_Redraw();
    Task_Wait(1);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 0);
    Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
    Actor_Jump(ACTOR_GERALD, 6, 0);
    Actor_Jump(ACTOR_IVAN, 6, 0);
    Actor_Jump(ACTOR_MIA, 6, 0);
    group_action = Data_0200e3c0;
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, group_action);
    Actor_EnableActionCallback( 1, group_action);
    Actor_EnableActionCallback( 2, group_action);
    Actor_EnableActionCallback( 3, group_action);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Event_Wait(80);
    Work_SetValuesIfNonNegative(0x60000, 0x60000, 0x10000);
    Event_Wait(40);
    Audio_PlayCue(145);
    Map_CopyCellAttributes(110, 106, 18, 14, 10, 5);
    Map_CopyCellsTo(110, 105, 74, 4, 18, 23);
    Map_CopyCellsTo(92, 86, 11, 68, 16, 20);
    rec = Pointer1(Engine_ActorGet, 0);
    *(s32 *)(rec + 8) += -0x100000;
    SceneActor_ParkRecord(rec);
    rec = Pointer1(Engine_ActorGet, 1);
    *(s32 *)(rec + 8) += -0x100000;
    SceneActor_ParkRecord(rec);
    rec = Pointer1(Engine_ActorGet, 2);
    *(s32 *)(rec + 8) += -0x100000;
    SceneActor_ParkRecord(rec);
    rec = Pointer1(Engine_ActorGet, 3);
    *(s32 *)(rec + 8) += -0x100000;
    SceneActor_ParkRecord(rec);
    rec = Pointer1(Engine_ActorGet, 8);
    *(s32 *)(rec + 8) += 0x100000;
    SceneActor_ParkRecord(rec);
    rec = Pointer1(Engine_ActorGet, 9);
    *(s32 *)(rec + 8) += 0x100000;
    SceneActor_ParkRecord(rec);
    rec = Pointer1(Engine_ActorGet, 10);
    *(s32 *)(rec + 8) += -0x100000;
    SceneActor_ParkRecord(rec);
    rec = Pointer1(Engine_ActorGet, 11);
    *(s32 *)(rec + 8) += -0x100000;
    SceneActor_ParkRecord(rec);
    Camera_MoveTo(0x1420000, 0x200000, 0xb40000, 0);
    Map_Redraw();
    Task_Wait(1);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
    Actor_Jump(ACTOR_GERALD, 6, 0);
    Actor_Jump(ACTOR_IVAN, 6, 0);
    Actor_Jump(ACTOR_MIA, 6, 0);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_Wait(80);
    Audio_PlayCue(0x121);
    Event_Wait(40);
    Actor_Stop(ACTOR_PARTY_LEADER);
    Actor_Stop(ACTOR_GERALD);
    Actor_Stop(ACTOR_IVAN);
    Actor_Stop(ACTOR_MIA);
    Event_Wait(120);
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 120);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 20);
    VinasuChojo_ShowMessage(2);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 0);
    VinasuChojo_ShowMessage(1);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 20);
    Actor_SetAnimation(ACTOR_MIA, 4);
    VinasuChojo_ShowMessage(3);
    Actor_Jump(ACTOR_IVAN, 2, 20);
    VinasuChojo_ShowMessage(2);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    VinasuChojo_FaceActor(1, 0);
    VinasuChojo_ShowMessage(1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    closing_action = Data_0200e39c;
    Actor_EnableActionCallback(ACTOR_GERALD, closing_action);
    Actor_EnableActionCallback( 2, closing_action);
    Actor_EnableActionCallback(ACTOR_MIA, closing_action);
    Event_Wait(60);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x110, 216);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x110, 254);
    Event_Wait(80);
    cell = (u32)&Data_03001ebc;
    work = *(struct SceneWork **)cell;
    work->request = 0x201;
    work->setup = 16;
    Event_CloseScreen();
    cell -= 48;
    Event_WaitForScreen();
    Event_Wait(80);
    Graphics_EnableObjLayerAndCallbacks();
    *(u16 *)((*(s32 *)cell + 0x12f4)) = none;
    *(u16 *)((*(s32 *)cell + 0x12f6)) = none;
    Call3(UiText_ShowCenteredMessage, 0x284f, 0, 0);
    ObjectDispatch_StopCallbacksAndHideLayers();
    Event_Wait(80);
}
