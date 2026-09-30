#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)

#include "KORIMA_MURA.H"

struct Obj {
    s32 f00, f04, f08, f0c, f10, f14, f18, f1c;
    s32 f20, f24, f28, f2c, f30, f34, f38, f3c;
    s32 f40, f44, f48, f4c, f50, f54, f58, f5c;
    s32 f60;
    u16 f64;
};

struct Ent {
    u8 pad00[6];
    u16 f06;
    s32 f08;
    u8 pad0c[4];
    s32 f10;
    u8 pad14[0x46];
    u8 f5a;
    u8 pad5b[13];
    struct Ent *f68;
};

struct Rec { u16 f00, f02, f04, f06; };

struct Ent_02000800 {
    s32 f00, f04, f08, f0c;
    u8 pad10[0x45];
    u8 f55;
};

struct Obj_020025d8 {
    s32 f00, f04, f08, f0c, f10, f14;
    s32 f18;
    s32 f1c, f20, f24, f28, f2c, f30, f34;
    s32 f38, f3c, f40;
};

struct Sub {
    u8 pad00[9];
    u8 f09;
    u8 pad0a[28];
    u8 f26;
};

struct Obj_02002608 {
    u8 pad00[0x18];
    s32 f18;
    u8 pad1c[7];
    u8 f23;
    u8 pad24[12];
    s32 f30;
    s32 f34;
    u8 pad38[24];
    struct Sub *f50;
    u8 pad54[1];
    u8 f55;
};

extern u8 KorimaMura_Object26Script[];

u16 ArcTan2(s32, s32);
void BattleFx_RunPageEffectForSlot(s32, s32, s32);
void BattleEffect_CleanupSceneObjects(void);

void SceneActor_SetActors19To22HeightByFrameParity(void)
{
    struct Ent_02000800 *p;

    p = Engine_ActorGet(19);
    if (p != 0) {
        s32 m;
        p->f55 = 0;
        m = gFrameCount & 1;
        if (m == 0) {
            p->f0c = m;
        } else {
            p->f0c = 0x1f40000;
        }
    }
    p = Engine_ActorGet(20);
    if (p != 0) {
        s32 z = 0;
        p->f55 = z;
        if (gFrameCount & 1) {
            p->f0c = z;
        } else {
            p->f0c = 0x1f40000;
        }
    }
    p = Engine_ActorGet(21);
    if (p != 0) {
        s32 m;
        p->f55 = 0;
        m = gFrameCount & 1;
        if (m == 0) {
            p->f0c = m;
        } else {
            p->f0c = 0x1f40000;
        }
    }
    p = Engine_ActorGet(22);
    if (p != 0) {
        s32 z = 0;
        p->f55 = z;
        if (gFrameCount & 1) {
            p->f0c = z;
        } else {
            p->f0c = 0x1f40000;
        }
    }
}

void FieldScene_StartEffect141Sequence(s32 arg0, s32 arg1)
{
    Psynergy_Begin(141, 1);
    Psynergy_SetTarget(arg0, arg1);
    Psynergy_RaiseHands();
    Psynergy_PlayEffect(1);
    Task_Wait(1);
}

void FieldScene_RunSequenceA(void)
{
    Psynergy_PlayEffect(2);
    Psynergy_LowerHands();
    BattleEffect_CleanupSceneObjects();
}

#include "OBJECT_RUNTIME.H"
extern u8 MsgKorimaHesAsStumpedAsWe[];
extern u8 MsgKorimaKnowThoseFieldsWere[];
extern u8 MsgKorimaQuiet[];
extern u8 MsgKorimaThatsReliefRobinThoughtYoud[];
extern u8 MsgKorimaWasOurPsynergy[];
extern u8 MsgKorimaWatchOutItsHappeningAgain[];
extern u8 MsgKorimaWhatIsIt[];
extern u8 MsgKorimaYoureRobinThereIsntMuch[];

void Object_RefreshSelectorById();
void FieldScene_RunPairedStepA();
void FieldScene_RunPairedStepB();
void Object_SetActionCallbackAndRefreshById();
void Audio_PlayCueFromEventWork();

extern u8 KorimaMura_ActionTable1[];
extern u8 KorimaMura_ActionTable2[];
extern u8 KorimaMura_ActionTable3[];
extern u8 KorimaMura_ActionTable4[];
extern u8 KorimaMura_ActionTable5[];
extern u8 KorimaMura_ActionTable6[];
extern u8 KorimaMura_ActionTable7[];

void FieldScene_RunExtendedActorSequence(void)
{
    struct ObjectRuntime *record;
    s32 flag_addr;
    s32 mask;
    s32 value;
    s32 action_a;
    s32 action_b;
    s32 step_addr;
    s32 work_addr;
    s32 action_c;
    s32 action_d;
    s32 step_next;

    Event_Begin();
    flag_addr = (s32)&KorimaMura_LayoutFlag;
    *(s32 *)flag_addr = GameFlag_IsSet(3);
    record = Engine_ActorGet(19);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Engine_ActorGet(20);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Engine_ActorGet(21);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Engine_ActorGet(22);
    Actor_SetSpriteFlags((s32)record, 0);
    Camera_MoveTo(0x680000, -1, 0x1000000, 0);
    Map_Redraw();
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x170000, 0xf70000);
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 121, 238);
    Actor_SetSpeed(ACTOR_GERALD, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_IVAN, 0x9999, 0x4ccc);
    record = ((struct ObjectRuntime *(*)())Engine_ActorGet)(0);
    if ((s32)record != 0) {
        Actor_SetPosition(ACTOR_GERALD, record->x, record->z);
    }
    record = ((struct ObjectRuntime *(*)())Engine_ActorGet)(0);
    if ((s32)record != 0) {
        Actor_SetPosition(ACTOR_IVAN, record->x, record->z);
    }
    Actor_EnableActionCallback(ACTOR_GERALD, (s32)KorimaMura_ActionTable1);
    Actor_EnableActionCallback(ACTOR_IVAN, (s32)KorimaMura_ActionTable2);
    if (*(s32 *)flag_addr != 0) {
        Actor_SetSpeed(ACTOR_MIA, 0x9999, 0x4ccc);
        record = ((struct ObjectRuntime *(*)())Engine_ActorGet)(0);
        if ((s32)record != 0) {
            Actor_SetPosition(ACTOR_MIA, record->x, record->z);
        }
        Actor_EnableActionCallback(ACTOR_MIA, (s32)KorimaMura_ActionTable3);
    }
    Object_RefreshSelectorById(2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 20);
    Actor_FaceDirection(ACTOR_IVAN, 0xe000, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 20);
    flag_addr = (s32)&KorimaMura_LayoutFlag;
    if (*(s32 *)flag_addr != 0) {
        Actor_FaceDirection(ACTOR_MIA, 0x2000, 0);
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 20);
    Actor_FaceDirection(ACTOR_IVAN, 0x2000, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 60);
    if (*(s32 *)flag_addr != 0) {
        Actor_FaceDirection(ACTOR_MIA, 0xe000, 0);
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    if (*(s32 *)flag_addr != 0) {
        Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
    }
    Event_Wait(40);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Event_SetMessage((s32)MsgKorimaQuiet);
    FieldScene_RunPairedStepA(1, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(30);
    Actor_WalkToAndWait(ACTOR_IVAN, 72, 0x11e);
    Actor_WalkToAndWait(ACTOR_IVAN, 72, 0x12e);
    Actor_WalkToAndWait(ACTOR_IVAN, 88, 0x136);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    if (*(s32 *)flag_addr != 0) {
        Actor_FaceActor(ACTOR_MIA, ACTOR_IVAN, 0);
    }
    Event_Wait(30);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    if (*(s32 *)flag_addr != 0) {
        Actor_SetAnimation(ACTOR_MIA, 3);
    }
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_IVAN, 0, 0);
    Event_Wait(20);
    FieldScene_StartEffect141Sequence(2, 9);
    Event_Wait(40);
    FieldScene_RunSequenceA();
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 40);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    ((struct ObjectRuntime *)Engine_ActorGet(2))->action_flags &= 254;
    Actor_WalkToAndWait(ACTOR_IVAN, 80, 0x136);
    mask = 1;
    Event_Wait(1);
    ((struct ObjectRuntime *)Engine_ActorGet(2))->action_flags |= mask;
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 40);
    FieldScene_RunPairedStepA(1, 20);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 40);
    FieldScene_RunPairedStepA(2, 20);
    if (*(s32 *)flag_addr != 0) {
        Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
    }
    Actor_FaceEachOther(ACTOR_IVAN, ACTOR_GERALD, 60);
    if (*(s32 *)flag_addr != 0) {
        Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    }
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    FieldScene_RunPairedStepA(1, 20);
    Actor_SetSpeed(ACTOR_IVAN, 0x8000, 0x4000);
    ((struct ObjectRuntime *)Engine_ActorGet(2))->action_flags &= 254;
    Actor_WalkToAndWait(ACTOR_IVAN, 72, 0x11e);
    Event_Wait(1);
    ((struct ObjectRuntime *)Engine_ActorGet(2))->action_flags |= mask;
    Actor_EnableActionCallback(ACTOR_IVAN, (s32)KorimaMura_ActionTable2);
    if (*(s32 *)flag_addr != 0) {
        Actor_ShowEmote(ACTOR_MIA, 0x105, 0);
        Event_Wait(60);
        FieldScene_RunPairedStepA(3, 20);
        Actor_SetAnimation(ACTOR_MIA, 3);
    } else {
        *(u16 *)(((s32)gWork + 0x1d8)) += 1;
    }
    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(20);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Event_Wait(10);
    FieldScene_RunPairedStepB(1, 0x2000, 10);
    FieldScene_RunPairedStepB(0, 0xa000, 10);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAnimation(ACTOR_GERALD, 3);
    } else {
        Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
        *(u16 *)(((s32)gWork + 0x1d8)) += 1;
    }
    FieldScene_RunPairedStepA(1, 40);
    FieldScene_RunPairedStepB(2, 0x2000, 40);
    FieldScene_RunPairedStepB(2, 0x8000, 20);
    FieldScene_RunPairedStepB(2, 0x4000, 40);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    FieldScene_RunPairedStepB(0, 0x6000, 60);
    value = 160;
    FieldScene_RunPairedStepB(3, 0x2000, 10);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 0);
    FieldScene_RunPairedStepB(0, (value << 8), 10);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    FieldScene_RunPairedStepB(0, 0x6000, 10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_SetMessage((s32)MsgKorimaWhatIsIt);
    FieldScene_RunPairedStepA(1, 10);
    FieldScene_RunPairedStepB(2, 0xc000, 20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    FieldScene_RunPairedStepA(2, 20);
    FieldScene_RunPairedStepB(1, 0, 20);
    FieldScene_RunPairedStepB(0, (value << 8), 40);
    FieldScene_RunPairedStepB(1, 0x4000, 20);
    FieldScene_RunPairedStepB(0, 0x6000, 30);
    FieldScene_RunPairedStepB(1, 0x6000, 20);
    FieldScene_RunPairedStepB(0, 0xe000, 30);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    FieldScene_RunPairedStepB(0, 0x6000, 20);
    FieldScene_RunPairedStepB(2, 0xc000, 10);
    Audio_PlayCue(17);
    Audio_PlayCue(206);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(1);
    Task_Wait(1);
    KorimaMura_EffectActive = 1;
    Engine_TaskAddCallback((s32)SceneEffect_SpawnObject26EveryEightFrames, 0xc80);
    Task_Wait(20);
    ColorBuffer_ApplyTarget(0x405210, 1);
    ColorBuffer_ApplyTarget(0x10000, 2);
    ColorBuffer_Interpolate(120);
    Task_Wait(60);
    action_a = (s32)KorimaMura_ActionTable4;
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, action_a);
    Actor_EnableActionCallback(ACTOR_GERALD, action_a);
    Actor_EnableActionCallback(ACTOR_IVAN, action_a);
    Actor_EnableActionCallback(ACTOR_MIA, action_a);
    Event_Wait(100);
    FieldScene_RunPairedStepA(1, 20);
    FieldScene_RunPairedStepA(2, 40);
    if (KorimaMura_LayoutFlag != 0) {
        Event_Wait(40);
        Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
        Event_Wait(40);
        FieldScene_RunPairedStepA(3, 40);
    } else {
        *(u16 *)(((s32)gWork + 0x1d8)) += 1;
    }
    Event_Wait(20);
    if (KorimaMura_LayoutFlag != 0) {
        value = 128;
        record = Engine_ActorGet(3);
        record->velocity_y = (value << 10);
        Event_Wait(10);
        Actor_SetSpeed(ACTOR_MIA, (value << 10), (value << 10));
        Actor_SetDestinationOffset(ACTOR_MIA, -2, 0);
        Actor_EnableActionCallback(ACTOR_MIA, (s32)KorimaMura_ActionTable5);
        record = Engine_ActorGet(3);
        Actor_SetSpriteFlags((s32)record, 0);
        Actor_SetAnimation(ACTOR_MIA, 19);
        Event_Wait(10);
    }
    value = 128;
    record = Engine_ActorGet(0);
    record->velocity_y = (value << 10);
    Event_Wait(10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, (value << 10), (value << 10));
    action_b = (s32)KorimaMura_ActionTable5;
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, action_b);
    record = Engine_ActorGet(0);
    Actor_SetSpriteFlags((s32)record, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 19);
    Event_Wait(20);
    record = ((struct ObjectRuntime *(*)())Engine_ActorGet)(1);
    record->velocity_y = (value << 10);
    Event_Wait(10);
    Actor_SetSpeed(ACTOR_GERALD, (value << 10), (value << 10));
    Actor_EnableActionCallback(ACTOR_GERALD, action_b);
    record = Engine_ActorGet(1);
    Actor_SetSpriteFlags((s32)record, 0);
    Actor_SetAnimation(ACTOR_GERALD, 19);
    Event_Wait(40);
    record = ((struct ObjectRuntime *(*)())Engine_ActorGet)(2);
    record->velocity_y = (value << 10);
    Event_Wait(10);
    Actor_EnableActionCallback(ACTOR_IVAN, action_b);
    record = Engine_ActorGet(2);
    Actor_SetSpriteFlags((s32)record, 0);
    Actor_SetAnimation(ACTOR_IVAN, 19);
    KorimaMura_EffectActive = 0;
    Event_Wait(160);
    Engine_TaskRemoveCallback((s32)SceneEffect_SpawnObject26EveryEightFrames);
    Event_Wait(120);
    ColorBuffer_ApplyTarget(0x406218, 1);
    ColorBuffer_Interpolate(60);
    Task_Wait(60);
    gFallingEffectWidth = 0;
    step_addr = (s32)&gFallingEffectState;
    gFallingEffectOffset = 0x800000;
    *(s32 *)step_addr = 1;
    Engine_TaskAddCallback((s32)FieldScene_UpdateFallingEffect, 0xc80);
    Event_Wait(180);
    Audio_PlayCue(21);
    FieldScene_RunPairedStepA(1, 80);
    FieldScene_RunPairedStepA(2, 40);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Event_Wait(60);
    FieldScene_RunPairedStepA(2, 20);
    *(s32 *)step_addr = 2;
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 1);
    Event_Wait(20);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 3);
    Event_Wait(40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Event_Wait(20);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    FieldScene_RunPairedStepA(1, 20);
    if (KorimaMura_LayoutFlag != 0) {
        Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
        FieldScene_RunPairedStepA(3, 10);
    } else {
        *(u16 *)(((s32)gWork + 0x1d8)) += 1;
    }
    work_addr = (s32)&gFallingEffectState;
    *(s32 *)work_addr = 3;
    ((struct ObjectRuntime *)Engine_ActorGet(0))->unknown_23 &= 254;
    ((struct ObjectRuntime *)Engine_ActorGet(1))->unknown_23 &= 254;
    ((struct ObjectRuntime *)Engine_ActorGet(2))->unknown_23 &= 254;
    ((struct ObjectRuntime *)Engine_ActorGet(3))->unknown_23 &= 254;
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
    Actor_SetSpritePriority(ACTOR_GERALD, 3);
    Actor_SetSpritePriority(ACTOR_IVAN, 3);
    value = 0;
    Actor_SetSpritePriority(ACTOR_MIA, 3);
    KorimaMura_PendingPose = value;
    Engine_TaskAddCallback((s32)SceneActor_SetActors19To22HeightByFrameParity, 0xc80);
    Audio_PlayCue(220);
    ((struct ObjectRuntime *)Engine_ActorGet(19))->unknown_23 &= 254;
    Actor_SetSpritePriority(19, 2);
    Actor_SetPosition(19, 0x780000, 0xf80000);
    action_c = (s32)KorimaMura_ActionTable6;
    Actor_EnableActionCallback(19, action_c);
    ((struct ObjectRuntime *)Engine_ActorGet(20))->unknown_23 &= 254;
    Actor_SetSpritePriority(20, 2);
    Actor_SetPosition(20, 0x640000, 0x1120000);
    Actor_EnableActionCallback(20, action_c);
    if (KorimaMura_LayoutFlag != 0) {
        ((struct ObjectRuntime *)Engine_ActorGet(21))->unknown_23 &= 254;
        Actor_SetSpritePriority(21, 2);
        Actor_SetPosition(21, 0x4a0000, 0xfe0000);
        Actor_EnableActionCallback(21, action_c);
    }
    ((struct ObjectRuntime *)Engine_ActorGet(22))->unknown_23 &= 254;
    Actor_SetSpritePriority(22, 2);
    Actor_SetPosition(22, 0x5e0000, 0xe10000);
    Actor_EnableActionCallback(22, action_c);
    if (*(s32 *)work_addr != 0) {
        do {
            Task_Wait(1);
        } while (gFallingEffectState != 0);
    }
    Event_Wait(0x12c);
    Engine_TaskRemoveCallback((s32)FieldScene_UpdateFallingEffect);
    Event_Wait(120);
    Audio_PlayCue(17);
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(60);
    Task_Wait(60);
    Actor_Stop(19);
    Actor_Stop(20);
    if (KorimaMura_LayoutFlag != 0) {
        Actor_Stop(21);
    }
    (Engine_ActorStop)(22);
    Task_Wait(1);
    action_d = (s32)KorimaMura_ActionTable7;
    (Engine_ActorEnableActionCallback)(19, action_d);
    Actor_EnableActionCallback(20, action_d);
    if (KorimaMura_LayoutFlag != 0) {
        Actor_EnableActionCallback(21, action_d);
    }
    Object_SetActionCallbackAndRefreshById(22, action_d);
    Event_Wait(80);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(40);
    Event_OpenMessage(ACTOR_GERALD, 0);
    Actor_SetPosition(17, 0x570000, 0x8b0000);
    Actor_SetPosition(18, 0x570000, 0x8b0000);
    Task_Wait(1);
    if (Event_ChooseYesNo(17, 0) == 1) {
        *(u16 *)(((s32)gWork + 0x1d8)) += 1;
    }
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    FieldScene_RunPairedStepA(2, 20);
    if (KorimaMura_LayoutFlag != 0) {
        Actor_RunRepeatedMotion(ACTOR_MIA, 2);
        Event_Wait(10);
        Event_SetMessage((s32)MsgKorimaKnowThoseFieldsWere);
        FieldScene_RunPairedStepA(3, 40);
    }
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Event_Wait(80);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_SetMessage((s32)MsgKorimaWasOurPsynergy);
    FieldScene_RunPairedStepA(2, 40);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 3);
    Event_Wait(40);
    Actor_SetSpritePriority(ACTOR_GERALD, 2);
    ((struct ObjectRuntime *)Engine_ActorGet(1))->unknown_23 |= 1;
    record = Engine_ActorGet(1);
    Actor_SetSpriteFlags((s32)record, 1);
    Actor_Jump(ACTOR_GERALD, 6, 0);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    FieldScene_RunPairedStepB(1, 0x4000, 60);
    FieldScene_RunPairedStepA(1, 20);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    (FieldScene_RunPairedStepA)(1, 10);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
    FieldScene_RunPairedStepB(1, 0x2000, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Event_Wait(40);
    FieldScene_RunPairedStepB(1, 0x6000, 40);
    FieldScene_RunPairedStepB(1, 0x2000, 20);
    FieldScene_RunPairedStepB(1, 0x6000, 20);
    FieldScene_RunPairedStepB(1, 0x2000, 10);
    Actor_Jump(ACTOR_GERALD, 2, 0);
    Event_Wait(40);
    Actor_Jump(ACTOR_GERALD, 2, 0);
    Event_Wait(10);
    Actor_Jump(ACTOR_GERALD, 4, 0);
    Event_Wait(20);
    FieldScene_RunPairedStepA(1, 20);
    if (KorimaMura_LayoutFlag != 0) {
        Actor_ShowEmote(ACTOR_MIA, 0x100, 0);
        Event_Wait(60);
        Actor_RunRepeatedMotion(ACTOR_MIA, 2);
        Event_Wait(80);
        Actor_SetSpritePriority(ACTOR_MIA, 2);
        ((struct ObjectRuntime *)Engine_ActorGet(3))->unknown_23 |= 1;
        record = Engine_ActorGet(3);
        Actor_SetSpriteFlags((s32)record, 1);
        Actor_Jump(ACTOR_MIA, 4, 0);
        Actor_SetDestinationOffset(ACTOR_MIA, -2, 0);
        Actor_SetAnimation(ACTOR_MIA, 1);
        FieldScene_RunPairedStepB(3, 0xe000, 60);
        Actor_RunRepeatedMotion(ACTOR_MIA, 2);
        Event_Wait(20);
        FieldScene_RunPairedStepA(3, 20);
    } else {
        *(u16 *)(((s32)gWork + 0x1d8)) += 1;
    }
    Actor_Jump(ACTOR_GERALD, 2, 0);
    FieldScene_RunPairedStepB(1, 0x4000, 20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    FieldScene_RunPairedStepB(1, 0x2000, 10);
    FieldScene_RunPairedStepA(1, 20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    value = 1;
    Actor_SetSpritePriority(ACTOR_IVAN, 2);
    ((struct ObjectRuntime *)Engine_ActorGet(2))->unknown_23 |= value;
    record = Engine_ActorGet(2);
    Actor_SetSpriteFlags((s32)record, 1);
    Actor_Jump(ACTOR_IVAN, 4, 0);
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(10);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
    ((struct ObjectRuntime *)Engine_ActorGet(0))->unknown_23 |= value;
    record = Engine_ActorGet(0);
    Actor_SetSpriteFlags((s32)record, 1);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    FieldScene_RunPairedStepB(0, 0x6000, 60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 0);
    Event_Wait(60);
    FieldScene_RunPairedStepB(0, 0xa000, 20);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    FieldScene_RunPairedStepB(0, 0x6000, 10);
    FieldScene_RunPairedStepB(1, 0x4000, 10);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    FieldScene_RunPairedStepA(2, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(20);
    Event_OpenMessage(ACTOR_IVAN, 0);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_FaceDirection(ACTOR_IVAN, 0xe000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimation(ACTOR_IVAN, 3);
        Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
        *(u16 *)(((s32)gWork + 0x1d8)) += 1;
    } else {
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        FieldScene_RunPairedStepB(1, 0x2000, 10);
        Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
        Event_ShowMessage(ACTOR_GERALD, 0);
    }
    FieldScene_RunPairedStepB(1, 0x4000, 10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    FieldScene_RunPairedStepA(1, 20);
    FieldScene_RunPairedStepB(2, 0xc000, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    FieldScene_RunPairedStepA(2, 10);
    if (KorimaMura_LayoutFlag != 0) {
        Actor_RunRepeatedMotion(ACTOR_MIA, 2);
        FieldScene_RunPairedStepB(3, 0, 20);
        FieldScene_RunPairedStepB(3, 0x2000, 10);
        Actor_SetAnimation(ACTOR_MIA, 4);
        FieldScene_RunPairedStepA(3, 10);
    } else {
        *(u16 *)(((s32)gWork + 0x1d8)) += 1;
    }
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 0);
    FieldScene_RunPairedStepB(0, 0xa000, 10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    value = 128;
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    FieldScene_RunPairedStepB(1, (value << 7), 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 80);
    FieldScene_RunPairedStepB(2, 0xe000, 10);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    FieldScene_RunPairedStepA(2, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 0);
    FieldScene_RunPairedStepB(0, 0xa000, 40);
    Actor_FaceDirection(ACTOR_GERALD, (value << 7), 0);
    FieldScene_RunPairedStepB(0, 0x6000, 10);
    FieldScene_RunPairedStepB(2, 0xc000, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    FieldScene_RunPairedStepA(2, 10);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Event_Wait(40);
    FieldScene_RunPairedStepA(1, 20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(20);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Event_Wait(40);
    FieldScene_RunPairedStepA(1, 20);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    FieldScene_RunPairedStepA(2, 10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    FieldScene_RunPairedStepB(1, 0x2000, 10);
    Event_OpenMessage(ACTOR_GERALD, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    } else {
        Event_Wait(20);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        Event_Wait(40);
        *(u16 *)(((s32)gWork + 0x1d8)) += 1;
    }
    Event_ShowMessage(ACTOR_GERALD, 0);
    Audio_PlayCue(21);
    ColorBuffer_ApplyTarget(0x406218, 1);
    ColorBuffer_Interpolate(60);
    Task_Wait(60);
    gFallingEffectWidth = 0;
    gFallingEffectOffset = 0x800000;
    step_next = (s32)&gFallingEffectState;
    *(s32 *)step_next = 1;
    Engine_TaskAddCallback((s32)FieldScene_UpdateFallingEffect, 0xc80);
    Event_Wait(80);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(60);
    FieldScene_RunPairedStepB(2, 0xc000, 10);
    Event_SetMessage((s32)MsgKorimaWatchOutItsHappeningAgain);
    FieldScene_RunPairedStepA(2, 10);
    FieldScene_RunPairedStepB(1, 0xc000, 10);
    FieldScene_RunPairedStepB(0, 0xc000, 10);
    if (KorimaMura_LayoutFlag != 0) {
        FieldScene_RunPairedStepB(3, 0xc000, 10);
    }
    ((struct ObjectRuntime *)Engine_ActorGet(0))->unknown_23 &= 254;
    ((struct ObjectRuntime *)Engine_ActorGet(1))->unknown_23 &= 254;
    ((struct ObjectRuntime *)Engine_ActorGet(2))->unknown_23 &= 254;
    ((struct ObjectRuntime *)Engine_ActorGet(3))->unknown_23 &= 254;
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
    Actor_SetSpritePriority(ACTOR_GERALD, 3);
    Actor_SetSpritePriority(ACTOR_IVAN, 3);
    Actor_SetSpritePriority(ACTOR_MIA, 3);
    *(s32 *)step_next = 2;
    Audio_PlayCue(220);
    Actor_SetPosition(19, 0x780000, 0xf80000);
    action_c = (s32)KorimaMura_ActionTable6;
    Actor_EnableActionCallback(19, action_c);
    Actor_SetPosition(20, 0x640000, 0x1120000);
    Actor_EnableActionCallback(20, action_c);
    if (KorimaMura_LayoutFlag != 0) {
        Actor_SetPosition(21, 0x4a0000, 0xfe0000);
        Actor_EnableActionCallback(21, action_c);
    }
    Actor_SetPosition(22, 0x5e0000, 0xe10000);
    Actor_EnableActionCallback(22, action_c);
    Event_Wait(120);
    *(s32 *)step_next = 3;
    do {
        Task_Wait(1);
    } while (gFallingEffectState != 0);
    FieldScene_RunPairedStepA(17, 80);
    FieldScene_RunPairedStepA(18, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 0);
    Event_Wait(60);
    FieldScene_RunPairedStepA(18, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    FieldScene_RunPairedStepA(18, 10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    FieldScene_RunPairedStepA(18, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 0);
    Event_Wait(40);
    FieldScene_RunPairedStepA(17, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0, 0);
    FieldScene_RunPairedStepB(2, 0xc000, 40);
    FieldScene_RunPairedStepA(18, 10);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    FieldScene_RunPairedStepB(3, 0xc000, 80);
    FieldScene_RunPairedStepA(18, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    FieldScene_RunPairedStepB(3, 0, 40);
    FieldScene_RunPairedStepA(17, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    FieldScene_RunPairedStepB(3, 0xc000, 10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 4);
    Actor_SetAnimation(ACTOR_GERALD, 4);
    Actor_SetAnimation(ACTOR_MIA, 4);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Event_Wait(60);
    FieldScene_RunPairedStepA(18, 10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    FieldScene_RunPairedStepA(18, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    FieldScene_RunPairedStepB(3, 0, 20);
    FieldScene_RunPairedStepA(18, 10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    FieldScene_RunPairedStepA(18, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    FieldScene_RunPairedStepB(3, 0, 20);
    FieldScene_RunPairedStepA(17, 20);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Event_Wait(40);
    FieldScene_RunPairedStepA(18, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    FieldScene_RunPairedStepB(3, 0xc000, 10);
    FieldScene_RunPairedStepA(18, 10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(60);
    Event_ShowMessage(18, 0);
    Event_ShowMessage(17, 0);
    Engine_TaskRemoveCallback((s32)FieldScene_UpdateFallingEffect);
    Event_Wait(80);
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(60);
    Task_Wait(80);
    Actor_Stop(19);
    Actor_Stop(20);
    work_addr = (s32)&KorimaMura_LayoutFlag;
    Actor_Stop(21);
    Actor_Stop(22);
    Task_Wait(1);
    action_d = (s32)KorimaMura_ActionTable7;
    Actor_EnableActionCallback(19, action_d);
    Actor_EnableActionCallback(20, action_d);
    if (*(s32 *)work_addr != 0) {
        Actor_EnableActionCallback(21, action_d);
    }
    Object_SetActionCallbackAndRefreshById(22, action_d);
    Event_Wait(20);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
    Actor_SetSpritePriority(ACTOR_GERALD, 2);
    Actor_SetSpritePriority(ACTOR_IVAN, 2);
    value = 1;
    Actor_SetSpritePriority(ACTOR_MIA, 2);
    ((struct ObjectRuntime *)Engine_ActorGet(0))->unknown_23 |= value;
    ((struct ObjectRuntime *)Engine_ActorGet(1))->unknown_23 |= value;
    ((struct ObjectRuntime *)Engine_ActorGet(2))->unknown_23 |= value;
    ((struct ObjectRuntime *)Engine_ActorGet(3))->unknown_23 |= value;
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    FieldScene_RunPairedStepB(2, 0xe000, 10);
    Event_OpenMessage(ACTOR_IVAN, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        Event_Wait(10);
        Event_OpenMessage(ACTOR_GERALD, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            FieldScene_RunPairedStepB(3, 0, 20);
            Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
            Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
            Actor_ShowEmote(ACTOR_MIA, 0x101, 0);
            Event_Wait(40);
            FieldScene_RunPairedStepB(1, 0x4000, 20);
            FieldScene_RunPairedStepA(1, 10);
            FieldScene_RunPairedStepB(2, 0xc000, 20);
            FieldScene_RunPairedStepB(2, 0xe000, 20);
            Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
            FieldScene_RunPairedStepA(2, 20);
            FieldScene_RunPairedStepB(1, 0x2000, 20);
        } else {
            FieldScene_RunPairedStepB(3, 0, 20);
            Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
            Actor_ShowEmote(ACTOR_IVAN, 0x102, 0);
            Actor_ShowEmote(ACTOR_MIA, 0x102, 40);
            FieldScene_RunPairedStepB(1, 0x4000, 20);
            Event_SetMessage((s32)MsgKorimaHesAsStumpedAsWe);
            FieldScene_RunPairedStepA(1, 20);
            Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
            FieldScene_RunPairedStepA(2, 20);
        }
        Actor_SetAnimation(ACTOR_MIA, 3);
        Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    } else {
        Event_Wait(20);
        Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
        Event_Wait(10);
        Event_SetMessage((s32)MsgKorimaYoureRobinThereIsntMuch);
        FieldScene_RunPairedStepA(1, 10);
        Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
        FieldScene_RunPairedStepB(0, 0x6000, 20);
        Actor_SetAnimation(ACTOR_GERALD, 3);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Event_Wait(10);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
        Event_OpenMessage(ACTOR_IVAN, 0);
        if (Event_ChooseYesNo(0, 0) != 0) {
            goto L_0200239c;
        }
        Event_Wait(20);
        Actor_ShowEmote(ACTOR_IVAN, 0x103, 0);
        Event_Wait(40);
        FieldScene_RunPairedStepB(2, 0xe000, 10);
        FieldScene_RunPairedStepA(2, 10);
        if (KorimaMura_LayoutFlag != 0) {
            FieldScene_RunPairedStepB(3, 0, 10);
            Actor_StartRepeatedMotion(ACTOR_MIA, 3);
            FieldScene_RunPairedStepA(3, 20);
        } else {
            *(u16 *)(((s32)gWork + 0x1d8)) += 1;
        }
        Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
        Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
        Event_Wait(40);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        FieldScene_RunPairedStepA(1, 20);
        Actor_ShowEmote(ACTOR_GERALD, 0x105, 0);
        Event_Wait(120);
        FieldScene_RunPairedStepA(2, 40);
        if (KorimaMura_LayoutFlag != 0) {
            FieldScene_RunPairedStepB(3, 0x2000, 10);
            Actor_SetAnimationAndWait(ACTOR_MIA, 4);
            FieldScene_RunPairedStepA(3, 10);
        } else {
            *(u16 *)(((s32)gWork + 0x1d8)) += 1;
        }
        Event_Wait(60);
        Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
        if (KorimaMura_LayoutFlag != 0) {
            FieldScene_RunPairedStepB(2, 0xa000, 40);
            FieldScene_RunPairedStepB(2, 0xe000, 20);
        }
        FieldScene_RunPairedStepA(2, 10);
        Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        Event_Wait(40);
        FieldScene_RunPairedStepA(2, 20);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
        Event_Wait(20);
        Actor_SetAnimation(ACTOR_MIA, 3);
    }
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    goto L_020024a0;
    L_0200239c:;
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_SetMessage((s32)MsgKorimaThatsReliefRobinThoughtYoud);
    FieldScene_RunPairedStepA(2, 20);
    if (KorimaMura_LayoutFlag != 0) {
        FieldScene_RunPairedStepB(3, 0, 10);
        Actor_StartRepeatedMotion(ACTOR_MIA, 1);
        FieldScene_RunPairedStepA(3, 20);
    } else {
        *(u16 *)(((s32)gWork + 0x1d8)) += 1;
    }
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    FieldScene_RunPairedStepA(1, 20);
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 0);
    Event_Wait(80);
    FieldScene_RunPairedStepA(2, 40);
    if (KorimaMura_LayoutFlag != 0) {
        FieldScene_RunPairedStepB(3, 0x2000, 20);
        Actor_SetAnimation(ACTOR_MIA, 4);
        FieldScene_RunPairedStepA(3, 40);
    } else {
        *(u16 *)(((s32)gWork + 0x1d8)) += 1;
    }
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    FieldScene_RunPairedStepA(2, 20);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(40);
    FieldScene_RunPairedStepA(2, 20);
    L_020024a0:;
    Audio_PlayCue(17);
    Actor_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_IVAN, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_MIA, 0x13333, 0x9999);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    record = ((struct ObjectRuntime *(*)())Engine_ActorGet)(0);
    if ((s32)record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetAnimation(ACTOR_IVAN, 2);
    record = ((struct ObjectRuntime *(*)())Engine_ActorGet)(0);
    if ((s32)record != 0) {
        Actor_SetDestination(ACTOR_IVAN, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(18, 0, 0);
    if (KorimaMura_LayoutFlag != 0) {
        Actor_SetAnimation(ACTOR_MIA, 2);
        record = ((struct ObjectRuntime *(*)())Engine_ActorGet)(0);
        if ((s32)record != 0) {
            Actor_SetDestination(ACTOR_MIA, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
        Actor_WaitForMove(ACTOR_MIA);
        Actor_SetPosition(ACTOR_MIA, 0, 0);
    }
    GameFlag_Set(0x843);
    Audio_PlayCueFromEventWork();
    Event_End();
}

void FieldScene_RunPairedStepA(s32 arg0, s32 arg1)
{
    Event_ShowMessage(arg0, 0);
    Event_Wait(arg1);
}

void FieldScene_RunPairedStepB(s32 arg0, s32 arg1, s32 arg2)
{
    Actor_FaceDirection(arg0, arg1, 0);
    Event_Wait(arg2);
}

s32 SceneEffect_AdvanceAngleUntilIdle(struct Obj_020025d8 *p)
{
    p->f18 += 0x1eb8;
    if (p->f38 == 0x80000000 && p->f3c == p->f38 && p->f40 == p->f3c) {
        Engine_ObjectDispatchRelease(p);
    }
    return 1;
}

void SceneEffect_SpawnObject26EveryEightFrames(void)
{
    struct Obj_02002608 *p;
    struct Sub *q;
    s32 f;
    s32 v;
    s32 w;
    s32 c1 = 0x620000;
    s32 c2 = 0x690000;
    s32 c3 = 0x620000;
    s32 c4 = 0x010d0000;

    f = gFrameCount & 7;
    if (f != 0) {
        return;
    }
    if (KorimaMura_EffectActive != 0) {
        Audio_PlayCue(200);
    }
    p = Engine_ObjectCreate(26, c1, 0, c2);
    if (p == 0) {
        return;
    }
    q = p->f50;
    q->f26 = f;
    v = 0xfe;
    v &= p->f23;
    p->f23 = v;
    w = ~12;
    w &= q->f09;
    w |= 4;
    q->f09 = w;
    p->f18 = 0x1999;
    p->f30 = 0x80000;
    p->f34 = 0x80000;
    p->f55 = f;
    Object_SetAnimation(p, 2);
    Engine_ObjectSetPosition(p, c3, 0, c4);
    Object_SetScript(p, KorimaMura_Object26Script);
}

s32 SceneEffect_SetModeByFrameBit1(s32 arg0)
{
    if ((gFrameCount >> 1) & 1) {
        Object_SetPalette(arg0, 10);
    } else {
        Object_SetPalette(arg0, 7);
    }
    return 0;
}
