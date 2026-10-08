#include "GLOBAL_CELLS.H"
#include "IMIRU.H"
#include "CALL.H"
#include "FRAME.H"
#include "FIELD_EVENT.H"

s32 __umodsi3();

void ImiruMura_ShowActorNineTurnMessage(void)
{
    extern s32 ImiruMura_TurnScript[];

    Engine_ActorEnableActionCallback(9, (s32)ImiruMura_TurnScript);
    Engine_EventShowMessage(9, 0);
}

void ImiruMura_CastPsynergyAtActorNine(void)
{
    u8 *work;

    Engine_PsynergyBegin(93, 1);
    work = *(u8 **)gEffectWork;
    Engine_PsynergySetTarget(3, 9);
    *(s32 *)(work + 36) = (s32)ImiruMura_ShowActorNineTurnMessage;
    Engine_PsynergyRaiseHands();
    Engine_PsynergyPlayEffect(1);
    Engine_PsynergyLowerHands();
    BattleEffect_CleanupSceneObjects();
}

void SceneActor_ResetActorAndCenterOffsets(struct Work_399 *work)
{
    struct Rec_399 *rec;

    work->f85 = 0;
    work->f100 = 0;
    work->f35 &= ~1;

    rec = work->f80;
    rec->mode = 1;

    ObjectGroup_SetChildValue(work, 9);
    Engine_ActorSetSpriteFlags(work, 0);

    work->f24 = 0x8000;
    work->f28 = 0x8000;
}

void FieldScene_RunSupplementalSequenceTwo(union FieldObject *object)
{
    object->effect.x += (s16)object->effect.spin << 8;
    object->effect.y += 0x8000;
    object->effect.scale_x += 0x7ae;
    object->effect.scale_y += 0x7ae;
    object->effect.spin += 2;
    if (--object->effect.countdown == 0) {
        Engine_ObjectDispatchRelease(object);
    }
}

void FieldScene_RunScene399SequenceA(void)
{
    volatile u32 *frame;
    union FieldObject *object;

    frame = &gFrameCount;
    if (__umodsi3(*frame, 60) == 0) {
        object = (union FieldObject *)Object_Create(222, 0x1cf0000, 0, 0x1240000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetMode(object, 5);
        }
    }
    if (__umodsi3(*frame + 30, 60) == 0) {
        object = (union FieldObject *)Object_Create(222, 0x1400000, 0x200000, 0x1640000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetMode(object, 5);
        }
    }
    if (__umodsi3(*frame + 10, 60) == 0) {
        object = (union FieldObject *)Object_Create(222, 0x760000, 0, 0x460000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetMode(object, 5);
        }
    }
    if (__umodsi3(*frame + 50, 60) == 0) {
        object = (union FieldObject *)Object_Create(222, 0x1560000, 0, 0x7c0000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetMode(object, 5);
        }
    }
    if (__umodsi3(*frame + 80, 60) == 0) {
        object = (union FieldObject *)Object_Create(222, 0x1af0000, 0, 0xab0000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetMode(object, 5);
        }
    }
}

void SceneState_UpdateZoneFlagsFromActorZero(void)
{
    T_020018c4 *obj;
    s32 x;
    s32 cx;
    s32 y;
    s32 r;
    s32 g;
    s32 h;
    State *st;

    obj = ((T *)Object_GetById(0));
    x = obj->unk8;
    cx = x >> 19;
    g = 0x200;
    h = 0x201;
    if ((u32)(cx - 24) > 7) {
        y = obj->unk10;
        if ((u32)((y >> 19) - 36) > 9 || (u32)(cx - 22) > 9)
            goto rest;
    }
    r = Engine_GameFlagIsSet(g);
    if (r != 0)
        return;
    ((State *)gMapWork[0])->unk17 = r;
    ((void (*)(s32))Engine_GameFlagSet)(g);
    Engine_GameFlagClear(h);
    return;

rest:
    if (x > 0xE80000 && obj->unkC > 0x1E0000 && y > 0xD40000) {
        st = gMapWork[0];
        st->unk17 = 0;
        ((void (*)(s32))Engine_GameFlagSet)(g);
        Engine_GameFlagClear(h);
        return;
    }
    r = Engine_GameFlagIsSet(h);
    if (r != 0)
        return;
    st = gMapWork[0];
    st->unk17 = 1;
    ((void (*)(s32))Engine_GameFlagSet)(h);
    Engine_GameFlagClear(g);
    return;
}

void SceneState_UpdateActor11WithFlag203(void)
{
    s32 a;
    s32 b;

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    struct FieldActor *actor = Object_GetById(11);

    a = actor->x.fixed >> 20;
    if (a != 15)
        return;
    b = actor->z.fixed >> 20;
    if (b != 7)
        return;
    Engine_GameFlagSet(0x203);
    Engine_ActorSetSpritePriority(11, 3);
#else
    Engine_GameFlagSet(0x203);
    Engine_ActorSetSpritePriority(11, 3);
    a = 15;
    b = 7;
#endif
    Engine_MapCopyCellAttributes(15, 6, 1, 1, a, b);
}

void SceneActor_SetActorZeroFacingC000AndRun(void)
{
    struct SceneService_02001990 *work;

    Engine_EventBegin();
    work = ((struct SceneService_02001990 *)Object_GetById(0));
    work->value06 = 0xc000;
    Engine_AudioPlayCue(123);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(8);
}

void RunEventScript02(void)
{
    u32 i;
    u8 *record;

    Engine_EventBegin();
    Engine_EventOpenScreen();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x1999);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 232, 204);
    Engine_EventEnd();
}

void FieldScene_RunThreeActorChoreography(void)
{

    struct FieldActor *leader;
    struct EventWork *work;

    Engine_EventBegin();
    Actor_FaceDirection(MEARI, 0xa000, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
    Engine_ActorWalkTo(0, 0x2b2, 200);

    ((struct FieldActor *)Battle_GetWorkObject1e0())->motion_flags = 0;
    Engine_CameraSetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x2b20000, 0, 0xa40000, 1);
    work = gEventWork;
    work->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    work->transition_frames = 48;
    Engine_EventOpenScreen();
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_SetSpeed(MEARI, 0x9999, 0x4ccc);
    leader = Object_GetById(0);
    if (leader != NULL) {
        Actor_SetPosition(MEARI, leader->x.fixed, leader->z.fixed);
    }
    Actor_WalkToAndWait(MEARI, 0x2a1, 183);
    Actor_FaceDirection(MEARI, 0xc000, 0);
    Engine_ActorStartRepeatedMotion(19, 2);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(40);
    Engine_EventSetMessage(MSGNO(MsgImiruMary));
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(MEARI, 0xe000, 40);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Actor_SetAttachedEffect(20, 0x102);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x4014, 0, 10);
    Actor_FaceDirection(MEARI, 0xa000, 40);
    Engine_ActorSetAnimationAndWait(MEARI, 4);
    Event_ShowMessageAndWait(0x2003, 0, 10);
    Engine_ActorSetAnimationAndWait(20, 3);
    Engine_EventWait(20);
    Actor_SetAttachedEffect(19, 0x102);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Actor_FaceDirection(MEARI, 0xf000, 10);
    Actor_FaceDirection(MEARI, 0x2000, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(MEARI, 0xc000, 40);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x4014, 0, 20);
    Actor_FaceDirection(MEARI, 0xa000, 20);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(60);
    Actor_ShowEmote(MEARI, 0x105, 60);
    Actor_ShowEmote(19, 0x101, 0);
    Actor_ShowEmote(20, 0x101, 60);
    Engine_ActorRunRepeatedMotion(19, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(MEARI, 0xe000, 40);
    Actor_FaceDirection(MEARI, 0xa000, 40);
    Actor_FaceDirection(MEARI, 0xe000, 20);
    Actor_FaceDirection(MEARI, 0x6000, 80);
    Event_ShowMessageAndWait(0x2003, 0, 20);
    Actor_FaceDirection(20, 0xf000, 0);
    Actor_FaceDirection(19, 0x7000, 40);
    Actor_FaceDirection(19, 0x5000, 0);
    Actor_FaceDirection(20, 0x3000, 20);
    Actor_SetSpeed(20, 0x10000, 0x8000);
    Object_GetById(20)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(20, 0x290, 166);
    Engine_EventWait(1);
    Object_GetById(20)->steering_flags |= ACTOR_FACES_MOTION;
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x4014, 0, 10);
    Engine_ActorRunRepeatedMotion(MEARI, 2);
    Engine_EventWait(40);
    Actor_FaceDirection(MEARI, 0xa000, 10);
    Event_ShowMessageAndWait(0x2003, 0, 40);
    Actor_FaceDirection(MEARI, 0x2000, 20);
    Event_ShowMessageAndWait(0x4003, 0, 10);
    Actor_SetAttachedEffect(19, 0x102);
    Actor_SetAttachedEffect(20, 0x102);
    Engine_EventWait(40);
    Actor_FaceDirection(MEARI, 0xc000, 20);
    Engine_ActorSetAnimation(MEARI, 4);
    Event_ShowMessageAndWait(0x2003, 0, 20);
    Engine_ActorRunRepeatedMotion(19, 1);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(MEARI, 0x2000, 40);
    Actor_FaceDirection(MEARI, 0xc000, 20);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(20, 1);
    Event_ShowMessageAndWait(0x4014, 0, 20);
    Engine_ActorRunRepeatedMotion(MEARI, 1);
    Engine_EventWait(20);
    Actor_FaceDirection(MEARI, 0xa000, 20);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Event_ShowMessageAndWait(0x2003, 0, 80);
    Actor_ShowEmote(19, 0x105, 0);
    Actor_ShowEmote(20, 0x105, 60);
    Engine_ActorSetAnimationAndWait(19, 4);
    Event_ShowMessageAndWait(19, 0, 10);
    Engine_ActorSetAnimation(20, 4);
    Event_ShowMessageAndWait(0x4014, 0, 20);
    Actor_ShowEmote(MEARI, 0x102, 60);
    Event_ShowMessageAndWait(0x2003, 0, 40);
    Engine_ActorSetAnimationAndWait(19, 3);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(MEARI, 0xe000, 20);
    Engine_ActorSetAnimationAndWait(20, 3);
    Event_ShowMessageAndWait(0x4014, 0, 10);
    Actor_FaceDirection(MEARI, 0xa000, 60);
    Actor_FaceDirection(MEARI, 0xe000, 20);
    Actor_FaceDirection(MEARI, 0xa000, 20);
    Actor_FaceDirection(MEARI, 0xc000, 40);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Event_ShowMessageAndWait(0x2003, 0, 10);
    Engine_ActorSetAnimation(19, 3);
    Engine_ActorSetAnimationAndWait(20, 3);
    Engine_EventWait(40);
    Actor_FaceDirection(MEARI, 0x2000, 20);
    Event_ShowMessageAndWait(0x4003, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(20);
    Actor_WalkToAndWait(MEARI, 0x2b0, 200);
    Actor_SetPosition(MEARI, 0, 0);
    Object_GetById(20)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(20, 0x284, 166);
    Engine_EventWait(1);
    *(u8 *)(((s32)Object_GetById(20)) + ACTOR_FLAGS_OFFSET_020019e8) |= 1;
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    GameFlag_Set(0x82e);
    GameFlag_Clear(0x82d);
    Engine_EventEnd();
}

void SceneActor_TurnTowardTableAngle(s32 z)
{
    T *o;
    s32 t;
    s32 d;
    u16 prev;
    s32 n;

    o = (T *)z;
    n = o->unk64;
    z = 0;
    t = ((s16 *)&o->unk64)[z];
    if (t != 0) {
        o->unk64 = n - 1;
        return;
    }
    o->unk5A = t;
    z = 1;
    d = ImiruMura_KeyHeadings[(*(u32 *)gKeysHeld >> 4) & 0xF];
    z = -z;
    if (d == z) {
        Object_SetMode(o, 9);
        return;
    }
    prev = o->unk6;
    d = (s16)(d - prev);
    if (d > 0x1000)
        d = 0x1000;
    if (d < -0x1000)
        d = -0x1000;
    o->unk6 = prev + d;
    Object_SetMode(o, 2);
    ObjectDispatch_ApplyValueToChildren(o, 0x30);
}

void StagedActor_RunHeadingProbeStep(void)
{
    extern struct SharedData_02000240 Data_02000240;

    struct Subject_02001fa4 *subject;
    s32 probe[3];
    s32 heading;
    s32 goal;
    s32 marker;
    s32 z;
    s32 x;
    u8 *p;

    subject = ObjectTable_Get(Data_02000240.selected_subject);

    for (;;) {
        heading = ImiruMura_SwayHeadings[(*(u32 *)gKeysHeld >> 4) & 15];
        if ((heading << 16) == -0x10000) {
            return;
        }

        Engine_EventBegin();

        probe[0] = (subject->x & 0xfff00000) + 0x80000;
        probe[1] = subject->y;
        probe[2] = (subject->z & 0xfff00000) + 0x80000;
        z = probe[2];
        x = probe[0];
        p = (u8 *)subject;
        p += 34;
        goal = GetMapCellCollision((s32)*p, x, z);
        Vector_AddPolarOffset((s32)0x100000, heading, probe);

        marker = GetMapCellCollision((s32)*p, probe[0], probe[2]);
        if (marker == 255
                || Map_GetTerrainHeight((s32)*p, probe[0], probe[2])
                    - subject->y > 0x80000) {
            subject->heading = (u16)heading;
            goto tail;
        }

        probe[0] = x;
        probe[2] = z;
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        subject->state_100 = 0;
        Engine_ObjectSetPosition(subject, x, subject->y, z);
        Object_SetMode(subject, 2);
        ObjectDispatch_ApplyValueToChildren(subject, 48);
        Engine_ObjectCommitPosition(subject);
        subject->callback = (void *)SceneActor_TurnTowardTableAngle;

        goto advanceProbe;
continueProbe:
        if (Map_GetTerrainHeight((s32)*p, probe[0], probe[2])
                - subject->y > 0x80000) {
            goto finishProbe;
        }
        x = probe[0];
        z = probe[2];
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        Engine_ObjectSetPosition(subject, probe[0], probe[1], probe[2]);
        Engine_ObjectCommitPosition(subject);
        if (marker != goal) {
            goto blocked;
        }

advanceProbe:
        ImiruMura_AdvanceProbe(heading, probe);
        marker = GetMapCellCollision((s32)*p, probe[0], probe[2]);
        if (marker != 255) {
            goto continueProbe;
        }

finishProbe:
        subject->state_048 = 0x20000;
        subject->state_052 = 0x10000;
        Engine_ObjectSetPosition(subject, x, subject->y, z);
        Engine_ObjectCommitPosition(subject);
        Engine_TaskWait(2);
    }

blocked:
    subject->callback = NULL;
    subject->flags_090 |= 1;

    subject->state_052 = 0x4000;

tail:
    Engine_TaskWait(10);
    Engine_EventEnd();
}
