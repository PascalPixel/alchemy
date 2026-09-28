#include "IMIRU.H"

void ActorPresentation_ApplyTableA5ecToActorNine(void)
{
    extern s32 ImiruMura_TurnScript[];

    Actor_EnableActionCallback(9, (s32)ImiruMura_TurnScript);
    Event_ShowMessage(9, 0);
}

void SceneState_StoreTable96adToWork(void)
{
    u8 *work;

    Psynergy_Begin(93, 1);
    work = *(u8 **)gEffectWork;
    Psynergy_SetTarget(3, 9);
    *(s32 *)(work + 36) = (s32)ActorPresentation_ApplyTableA5ecToActorNine;
    Psynergy_RaiseHands();
    Psynergy_PlayEffect(1);
    Psynergy_LowerHands();
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

    Object_SetPalette(work, 9);
    Actor_SetSpriteFlags(work, 0);

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
    extern u32 Data_03001e40;
    u32 *frame;
    union FieldObject *object;

    frame = &Data_03001e40;
    if (Value2_02001794(Engine_MathModulo, *frame, 60) == 0) {
        object = (union FieldObject *)Value4(Engine_ObjectCreate, 222, 0x1cf0000, 0, 0x1240000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetAnimation(object, 5);
        }
    }
    if (Value2_02001794(Engine_MathModulo, *frame + 30, 60) == 0) {
        object = (union FieldObject *)Value4(Engine_ObjectCreate, 222, 0x1400000, 0x200000, 0x1640000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetAnimation(object, 5);
        }
    }
    if (Value2_02001794(Engine_MathModulo, *frame + 10, 60) == 0) {
        object = (union FieldObject *)Value4(Engine_ObjectCreate, 222, 0x760000, 0, 0x460000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetAnimation(object, 5);
        }
    }
    if (Value2_02001794(Engine_MathModulo, *frame + 50, 60) == 0) {
        object = (union FieldObject *)Value4(Engine_ObjectCreate, 222, 0x1560000, 0, 0x7c0000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetAnimation(object, 5);
        }
    }
    if (Value2_02001794(Engine_MathModulo, *frame + 80, 60) == 0) {
        object = (union FieldObject *)Value4(Engine_ObjectCreate, 222, 0x1af0000, 0, 0xab0000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetAnimation(object, 5);
        }
    }
}

/* The flag setter is called through a void function type, as the three
 * call sites discard its result and the compiler keeps their tails apart. */
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

    obj = ((T *)Engine_ActorGet(0));
    x = obj->unk8;
    cx = x >> 19;
    g = 0x200;
    h = 0x201;
    if ((u32)(cx - 24) > 7) {
        y = obj->unk10;
        if ((u32)((y >> 19) - 36) > 9 || (u32)(cx - 22) > 9)
            goto rest;
    }
    r = GameFlag_IsSet(g);
    if (r != 0)
        return;
    (*(State **)&gMapWork)->unk17 = r;
    ((void (*)(s32))Engine_GameFlagSet)(g);
    GameFlag_Clear(h);
    return;

rest:
    if (x > 0xE80000 && obj->unkC > 0x1E0000 && y > 0xD40000) {
        st = *(State **)&gMapWork;
        st->unk17 = 0;
        ((void (*)(s32))Engine_GameFlagSet)(g);
        GameFlag_Clear(h);
        return;
    }
    r = GameFlag_IsSet(h);
    if (r != 0)
        return;
    st = *(State **)&gMapWork;
    st->unk17 = 1;
    ((void (*)(s32))Engine_GameFlagSet)(h);
    GameFlag_Clear(g);
    return;
}


void SceneState_UpdateActor11WithFlag203(void)
{
    s32 a;
    s32 b;

    GameFlag_Set(0x203);
    Actor_SetSpritePriority(11, 3);
    a = 15;
    b = 7;
    Map_CopyCellAttributes(15, 6, 1, 1, a, b);
}

void SceneActor_SetActorZeroFacingC000AndRun(void)
{
    struct SceneService_02001990 *work;

    Event_Begin();
    work = ((struct SceneService_02001990 *)Engine_ActorGet(0));
    work->value06 = 0xc000;
    Audio_PlayCue(123);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(8);
}

/* Runs a short fixed sequence of two calls, one 3-argument call passing a
 * fixed-point-looking pair of constants, one 3-argument call passing
 * (0, 232, 204), and a final call, in that order. */
void RunEventScript02(void)
{
    u32 i;
    u8 *record;

    Event_Begin();
    Event_OpenScreen(); /* main:0808a360 */
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x1999);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 232, 204);
    Event_End();
}

/* Runs a long scripted sequence of position, animation, and timing calls
 * against actor records 0, 3, 19, and 20, with a scene phase word at
 * offset 0x1c0 of the shared scene work record set at the start and near
 * the end. */
void FieldScene_RunThreeActorChoreography(void)
{

    struct FieldActor *leader;
    struct EventWork *work;

    Event_Begin();
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
    Value3(Engine_ActorWalkTo, 0, 0x2b2, 200);
    /* Clear the byte at offset 85 of the returned record. */
    *(u8 *)(Battle_GetWorkObject1e0() + 85) = 0;
    Value2(Engine_CameraSetSpeed, 0xcccc, 0x1999);
    Camera_MoveTo(0x2b20000, 0, 0xa40000, 1);
    work = *(struct EventWork **)Data_03001ebc;
    work->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    work->transition_frames = 48;
    Event_OpenScreen();
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_SetSpeed(ACTOR_MIA, 0x9999, 0x4ccc);
    leader = (struct FieldActor *)Value1(Engine_ActorGet, 0);
    if (leader != NULL) {
        Actor_SetPosition(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
    }
    Actor_WalkToAndWait(ACTOR_MIA, 0x2a1, 183);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_StartRepeatedMotion(19, 2);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(40);
    Event_SetMessage(MSG_MIA);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 40);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Actor_SetAttachedEffect(20, 0x102);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4014, 0, 10);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 40);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Event_ShowMessageAndWait(0x2003, 0, 10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(20);
    Actor_SetAttachedEffect(19, 0x102);
    Event_Wait(20);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xf000, 10);
    Actor_FaceDirection(ACTOR_MIA, 0x2000, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 40);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4014, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 20);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(60);
    Actor_ShowEmote(ACTOR_MIA, 0x105, 60);
    Actor_ShowEmote(19, 0x101, 0);
    Actor_ShowEmote(20, 0x101, 60);
    Actor_RunRepeatedMotion(19, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 40);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 40);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 20);
    Actor_FaceDirection(ACTOR_MIA, 0x6000, 80);
    Event_ShowMessageAndWait(0x2003, 0, 20);
    Actor_FaceDirection(20, 0xf000, 0);
    Actor_FaceDirection(19, 0x7000, 40);
    Actor_FaceDirection(19, 0x5000, 0);
    Actor_FaceDirection(20, 0x3000, 20);
    Actor_SetSpeed(20, 0x10000, 0x8000);
    *(u8 *)(((s32)Engine_ActorGet(20)) + ACTOR_FLAGS_OFFSET_020019e8) &= 254;
    Actor_WalkToAndWait(20, 0x290, 166);
    Event_Wait(1);
    *(u8 *)(((s32)Engine_ActorGet(20)) + ACTOR_FLAGS_OFFSET_020019e8) |= 1;
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4014, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 10);
    Event_ShowMessageAndWait(0x2003, 0, 40);
    Actor_FaceDirection(ACTOR_MIA, 0x2000, 20);
    Event_ShowMessageAndWait(0x4003, 0, 10);
    Actor_SetAttachedEffect(19, 0x102);
    Actor_SetAttachedEffect(20, 0x102);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 20);
    Actor_SetAnimation(ACTOR_MIA, 4);
    Event_ShowMessageAndWait(0x2003, 0, 20);
    Actor_RunRepeatedMotion(19, 1);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(ACTOR_MIA, 0x2000, 40);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 20);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(20, 1);
    Event_ShowMessageAndWait(0x4014, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 20);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_ShowMessageAndWait(0x2003, 0, 80);
    Actor_ShowEmote(19, 0x105, 0);
    Actor_ShowEmote(20, 0x105, 60);
    Actor_SetAnimationAndWait(19, 4);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_SetAnimation(20, 4);
    Event_ShowMessageAndWait(0x4014, 0, 20);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 60);
    Event_ShowMessageAndWait(0x2003, 0, 40);
    Actor_SetAnimationAndWait(19, 3);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 20);
    Actor_SetAnimationAndWait(20, 3);
    Event_ShowMessageAndWait(0x4014, 0, 10);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 60);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 20);
    Actor_FaceDirection(ACTOR_MIA, 0xa000, 20);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 40);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_ShowMessageAndWait(0x2003, 0, 10);
    Actor_SetAnimation(19, 3);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_MIA, 0x2000, 20);
    Event_ShowMessageAndWait(0x4003, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 20);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_WalkToAndWait(ACTOR_MIA, 0x2b0, 200);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    *(u8 *)(((s32)Engine_ActorGet(20)) + ACTOR_FLAGS_OFFSET_020019e8) &= 254;
    Actor_WalkToAndWait(20, 0x284, 166);
    Event_Wait(1);
    *(u8 *)(((s32)Engine_ActorGet(20)) + ACTOR_FLAGS_OFFSET_020019e8) |= 1;
    (*(struct EventWork **)Data_03001ebc)->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    GameFlag_Set(0x82e);
    GameFlag_Clear(0x82d);
    Event_End();
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
        Object_SetAnimation(o, 9);
        return;
    }
    prev = o->unk6;
    d = (s16)(d - prev);
    if (d > 0x1000)
        d = 0x1000;
    if (d < -0x1000)
        d = -0x1000;
    o->unk6 = prev + d;
    Object_SetAnimation(o, 2);
    ObjectDispatch_ApplyValueToChildren(o, 0x30);
}

/*
 * The overlay's pathing step for resource_399.  The six pool words after the
 * return belong to this owner.
 *
 * Frame map: the goal marker, then the heading, then the three-word probe
 * position that the stepping imports are handed by address.  The order of the x
 * and z assignments, together with the inline stepping wrapper, is what leaves
 * a single high-register copy before the arithmetic that follows it.
 */
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
        heading = ImiruMura_SwayHeadings[(Data_03001ae8 >> 4) & 15];
        /* The test is on heading << 16 against 0xffff0000, that is on the
         * signed halfword -1, which means "no heading". */
        if ((heading << 16) == (s32)0xffff0000) {
            return;
        }
        /* Nothing is placed in an argument register before this call. */
        Event_Begin();

        /* The 0x80000 bias is built from an immediate and a shift. */
        probe[0] = (subject->x & (s32)0xfff00000) + 0x80000;
        probe[1] = subject->y;
        probe[2] = (subject->z & (s32)0xfff00000) + 0x80000;
        z = probe[2];
        x = probe[0];
        p = (u8 *)subject;
        p += 34;
        goal = GetMapCellCollision((s32)*p, x, z);
        /* 0x100000 is built from an immediate and a shift.  The probe block is
         * passed by address and is advanced by the callee. */
        Vector_AddPolarOffset((s32)0x100000, heading, probe);

        marker = GetMapCellCollision((s32)*p, probe[0], probe[2]);
        if (marker == 255
                || Map_GetTerrainHeight((s32)*p, probe[0], probe[2])
                    - subject->y > 0x80000) {
            subject->heading = (u16)heading;
            goto tail;
        }

        /* Rewind the probe to the position it held before the step above. */
        probe[0] = x;
        probe[2] = z;
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        subject->state_100 = 0;
        Engine_ObjectSetPosition(subject, x, subject->y, z);
        /* Same import as the probe above, two arguments here. */
        Object_SetAnimation(subject, 2);
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
        AdvanceProbe_02001fa4(heading, probe);
        marker = GetMapCellCollision((s32)*p, probe[0], probe[2]);
        if (marker != 255) {
            goto continueProbe;
        }

finishProbe:
        subject->state_048 = 0x20000;
        subject->state_052 = 0x10000;
        Engine_ObjectSetPosition(subject, x, subject->y, z);
        Engine_ObjectCommitPosition(subject);
        Task_Wait(2);
        /* The back edge re-reads the heading table and starts again. */
    }

blocked:
    subject->callback = NULL;
    subject->flags_090 |= 1;
    /* 0x4000 is built from an immediate and a shift. */
    subject->state_052 = 0x4000;

tail:
    Task_Wait(10);
    Event_End();
}
