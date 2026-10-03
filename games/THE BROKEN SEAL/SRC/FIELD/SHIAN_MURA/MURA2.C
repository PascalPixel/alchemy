#include "SHIAN.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "CALL.H"

struct SceneState { u8 unk_00[14]; s16 field_0e; };

struct Actor { u8 unk_00[35]; u8 presentation_flags; };

struct MapActor {
    u8 reserved_00[6];
    u16 heading;
    s32 x;
    u8 reserved_0c[4];
    s32 z;
    u8 reserved_14[71];
    u8 tracking_state;
    u8 reserved_5c[8];
    s16 tracking_mode;
};

s32 ArcTan2(s32 z_delta, s32 x_delta);

extern u8 MsgShianDoingMadeMeSpillMy[];
extern u8 MsgShianNowMustGetWaterAgain[];
extern u8 MsgShianYoungWarriorsVeryGallantCame[];

extern u8 MsgShianHooWhaaaHachaaa[];
extern u8 MsgShianKungFuStrong[];
void Engine_EventBegin();
s32 Engine_GameFlagIsSet();
void Engine_ActorFaceActor();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_ActorFaceDirection();
void Engine_EventEnd();
void Vector_AddPolarOffset();
void Engine_ActorSetAnimation();
void Engine_ActorSetDestinationOffset();
void Engine_ActorWaitForMove();
void Effect_Spawn();
void Engine_AudioPlayCue();
s32 Engine_TaskAddCallback();
void Engine_ActorSetSpeed();
void Engine_ActorSetDestination();
void Object_RefreshSelectorById();
void Engine_EventShowMessageAndWait();
void BattleFx_PlayQueuedSound();

struct SpawnParams {
    s32 count;
    u8 unknown_04[20];
    u16 sprite;
    u16 pad_1a;
    s32 callback;
    u16 facing;
    u8 unknown_22[6];
};

void SceneEffect_SpawnPeriodicEffect();
void SceneEffect_AdvanceRotatingSprite();
extern const s32 ShianMura_Actor19Motion[];
extern const s32 ShianMura_EffectScript[];

extern u8 MsgShianWarriorsFromSchoolStrongWarriors[];

void Vector_AddPolarOffset(s32 distance, s32 angle, union FieldCoordinate *pos);
s32 Object_CheckMovementCollision(struct FieldActor *actor, union FieldCoordinate *pos);

extern u8 MsgShianWhaWhaHappened[];

void BattleFx_SetQueuedSoundAndPlay(s32 music);
void Actor_UpdatePresentationFlag(void);

s32 SceneEffect_CalculatePositionDistance( s32 *first_position, s32 *second_position);
s32 ShianMura_WatchGateTrigger(struct FieldActor *self);
s32 SceneActor_UpdateTracking( struct MapActor *actor, struct MapActor *target, s32 distance_limit, s32 force_tracking);

/* Plays page effect 22 for slot 1 with mode 2; all three arguments are
 * immediates, so the function carries no literal pool. */
void SceneEffect_RequestFixedEffect(void)
{
    BattleFx_RunPageEffectForSlot(22, 1, 2);
}

/*
 * Shian village: toggle the presentation flag on actor 20. While the scene
 * counter has passed the threshold the flag is set; otherwise it is cleared.
 *
 * The local SceneState and Actor layouts are this overlay's own copies of the
 * scene counter record and the field actor record (presentation_flags at
 * 0x24). The offsets here are what the reference reads, so the spelling is
 * kept as-is.
 */
void Actor_UpdatePresentationFlag(void)
{
    struct SceneState *scene = (void *)Object_GetById(0);
    if (scene->field_0e > 31) {
        ((struct Actor *)Object_GetById(20))->presentation_flags |= 2;
    } else {
        ((struct Actor *)Object_GetById(20))->presentation_flags &= (u8)~2;
    }
}

/* Complete entity-19 sprite-counter adjustment. */
void SceneEffect_AdvanceRotatingSprite(void)
{
    u8 *entity = Object_GetById(19);
    u8 *sprite = *(u8 **)(entity + 80);
    *(u16 *)(sprite + 30) += 0x1400;
}

void SceneEffect_SpawnPeriodicEffect(void)
{
    u8 *entity = Object_GetById(14);

    if ((gFrameCount & 3) == 0) {
        struct PeriodicEffectConfig config;
        config.kind = 1;
        config.variant = 9;
        config.id = 169;
        config.data = ShianMura_EffectConfig;
        Effect_Spawn(
            *(s32 *)(entity + 8),
            *(s32 *)(entity + 12),
            *(s32 *)(entity + 16) - 0x10000,
            0,
            -0x10000,
            -0x10000,
            0x330000,
            &config);
    }
}

/*
 * Distance between two three-component 16.16 fixed-point positions.
 *
 * Each argument walks three consecutive 16.16 words in x, y, z order. The
 * per-axis deltas are taken in fixed point, shifted down to integers, squared,
 * and summed; the total is passed to the resident IWRAM integer square root.
 *
 * Expressions are preserved exactly as reconstructed: the walking-pointer form
 * is load-bearing for byte-identity and must not become struct field access.
 */
s32 SceneEffect_CalculatePositionDistance(
    s32 *first_position,
    s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return Iwram_Sqrt(delta_x_squared + delta_y_squared + delta_z_squared);
}

/* Decides whether an actor tracks its target. An actor already tracking in
 * mode 0 carries on. Otherwise, when the target is closer than the distance
 * limit and lies in the actor's facing sector or one beside it, or tracking
 * is forced, the actor starts tracking in mode 1; if not, it stops tracking.
 * Returns 1 while tracking. */
s32 SceneActor_UpdateTracking(
    struct MapActor *actor,
    struct MapActor *target,
    s32 distance_limit,
    s32 force_tracking)
{
    s32 result = 0;

    if (actor->tracking_state == 1 && actor->tracking_mode == 0) {
        Object_SetMode(actor, 1);
        return 1;
    }

    if (SceneEffect_CalculatePositionDistance(&target->x, &actor->x) < distance_limit ||
        force_tracking != 0) {
        u16 angle = ArcTan2(target->z - actor->z, target->x - actor->x);
        s32 prev = (angle - 0x1000) & 0xf000;
        s32 next = (angle + 0x1000) & 0xf000;
        s32 facing = angle & 0xf000;
        s32 heading = actor->heading & 0xf000;

        if (facing == heading || next == heading || prev == heading ||
            force_tracking != 0) {
            actor->tracking_state = 1;
            Object_SetMode(actor, 1);
            result = 1;
            actor->tracking_mode = result;
        } else {
            actor->tracking_state = 0;
            Object_SetMode(actor, 2);
            actor->tracking_mode = 0;
        }
    } else {
        actor->tracking_state = 0;
        Object_SetMode(actor, 2);
        actor->tracking_mode = 0;
    }

    return result;
}

/* Counts the frames the leader stands at the gate beside this actor and raises trigger 200 after two seconds. */
s32 ShianMura_WatchGateTrigger(struct FieldActor *self)
{
    struct FieldActor *leader;
    s32 near = 0;

    if (self->target_x == ACTOR_NO_TARGET && self->target_z == ACTOR_NO_TARGET)
        return 0;
    leader = Object_GetById(0);
    if ((u32)((leader->x.fixed >> 20) - 17) <= 1 && leader->z.fixed >> 20 == 14 && self->x.fixed >> 20 <= 19
        && self->velocity_x <= 0) {
        if (leader->x.fixed <= self->x.fixed) {
            self->rise_counter++;
            near = 1;
        }
    } else {
        self->rise_counter = 0;
    }
    if (near && self->rise_counter > 119) {
        gEventWork->raised_trigger = 200;
        self->rise_counter = 0;
    }
    SceneActor_UpdateTracking(self, leader, 18, near);
    return 0;
}

u8 *SceneEffect_GetPrimaryData(void)
{
    return gShianMuraEntrances;
}

s32 SceneEffect_GetInitialValue(void)
{
    return 0;
}

u8 *SceneEffect_GetSecondaryData(void)
{
    return gShianMuraExits;
}

s32 SceneEffect_PrepareState(void)
{
    if (Engine_GameFlagIsSet(0x895) != 0)
        gShianMuraPlacements[0xbe] = 0;
    return (s32)gShianMuraPlacements;
}

void SceneEffect_ShowActorSetupMessage(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgShianYoungWarriorsVeryGallantCame);
    Engine_EventAskYesNo(9, 0);
    Engine_EventEnd();
}

void FieldScene_RunPrimarySequence(void)
{
    u32 i;
    s32 rec7;
    struct FieldActor *actor;
    u8 *record;
    s32 v7;
    s32 v5;
    s32 p5;
    s32 q1;
    s32 t2;
    s32 q2;
    s32 hi;
    s32 lo;

    actor = (struct FieldActor *)Object_GetById(20);
    Engine_EventBegin();
    v7 = 0;
    record = Actor_Get(18);
    *(s32 *)((s32)record + 108) = v7;
    if (GameFlag_IsSet(0x200) == 0) {
        record = Object_GetById(18);
        if ((*(s32 *)((s32)record + 8) >> 20) > 19) {
            goto L_020006a2;
        }
    }
    record = Actor_Get(18);
    p5 = *(u16 *)((s32)record + 6);
    Actor_FaceActor(18, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(10);
    Engine_EventSetMessage((s32)MsgShianNowMustGetWaterAgain);
    if (GameFlag_IsSet(0x200) == 0) {
        bump_step(1);
        Event_ShowMessage(18, 0);
        *(u16 *)((u8 *)Object_GetById(18) + 100) = v7;
        record = Object_GetById(18);
        *(u16 *)((s32)record + 6) = p5;
    } else {
        Event_ShowMessage(18, 0);
        Actor_FaceDirection(18, 0x8000, 20);
    }
    record = Actor_Get(18);
    *(s32 *)((s32)record + 108) = (s32)ShianMura_WatchGateTrigger;
    Engine_EventEnd();
    goto L_02000916;
    L_020006a2:;
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if ((*(s32 *)((s32)record + 16) >> 19) > 27) {
        record = Object_GetById(ACTOR_PARTY_LEADER);
        if ((*(s32 *)((s32)record + 16) >> 19) <= 29) {
            record = Object_GetById(ACTOR_PARTY_LEADER);
            if ((*(s32 *)((s32)record + 8) >> 20) != 26) {
                Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
                Engine_ActorFaceActor(0, 18, 0);
                Engine_EventWait(5);
                rec7 = Object_GetById(ACTOR_PARTY_LEADER);
                record = Actor_Get(18);
                if (*(s32 *)(rec7 + 8) < *(s32 *)((s32)record + 8)) {
                    *(u8 *)((u8 *)Object_GetById(0) + 90) &= 254;
                    record = Object_GetById(18);
                    Actor_WalkTo(ACTOR_PARTY_LEADER, (((*(s32 *)((s32)record + 8) >> 20) << 4) - 8), 232);
                    v7 = 1;
                } else {
                    *(u8 *)((u8 *)Object_GetById(0) + 90) &= 254;
                    record = Object_GetById(18);
                    Actor_WalkTo(ACTOR_PARTY_LEADER, (((*(s32 *)((s32)record + 8) >> 20) << 4) + 24), 232);
                }
                Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
            }
        }
    }
    v5 = 128;
    record = Actor_Get(18);
    *(s32 *)((s32)record + 56) = (v5 << 24);
    record = Object_GetById(18);
    *(s32 *)((s32)record + 60) = (v5 << 24);
    record = Actor_Get(18);
    *(s32 *)((s32)record + 64) = (v5 << 24);
    Engine_ActorEnableActionCallback(18, 1);
    Engine_ActorSetAnimation(18, 1);
    Engine_ActorRunRepeatedMotion(18, 2);
    Engine_EventWait(10);
    Audio_PlayCue(228);
    actor->scale_x = 0x4ccc;
    actor->scale_y = 0x4ccc;
    record = Object_GetById(18);
    q1 = *(s32 *)((s32)record + 8);
    record = Object_GetById(18);
    t2 = *(s32 *)((s32)record + 16) >> 20;
    Actor_SetPosition(20, (((q1 >> 20) << 20) + 0x80000), ((t2 << 20) + 0x80000));
    record = Object_GetById(18);
    q2 = *(s32 *)((s32)record + 8);
    record = Object_GetById(18);
    Map_CopyCellAttributes(16, 16, 1, 1, (q2 >> 20), (*(s32 *)((s32)record + 16) >> 20));
    Engine_ActorSetSpritePriority(20, 2);
    actor->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
    do {
        Engine_TaskWait(3);
        hi = actor->scale_y;
        lo = actor->scale_x;
        actor->scale_y = hi + 0x1999;
        lo += 0x1999;
        actor->scale_x = lo;
    } while (lo <= 0xffff);
    Actor_ShowEmote(18, 0x105, 70);
    Actor_FaceActor(18, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(18, 0x103, 0);
    Engine_ActorStartRepeatedMotion(18, 2);
    Engine_EventWait(70);
    Engine_EventSetMessage((s32)MsgShianDoingMadeMeSpillMy);
    Event_ShowMessageAndWait(18, 0, 20);
    BattleFx_PlayQueuedSound();
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if ((*(s32 *)((s32)record + 8) >> 20) == 26) {
        record = Object_GetById(ACTOR_PARTY_LEADER);
        if ((*(s32 *)((s32)record + 16) >> 20) > 13) {
            v7 = 1;
        }
    }
    if (v7 != 0) {
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 10);
        *(u8 *)((u8 *)Object_GetById(0) + 90) &= 254;
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
        Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
        Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    }
    Actor_SetSpeed(18, 0xcccc, 0x6666);
    record = Object_GetById(18);
    if ((*(s32 *)((s32)record + 16) >> 20) != 14) {
        record = Actor_Get(18);
        Actor_WalkToAndWait(18, *(s16 *)((s32)record + 10), 232);
    }
    Actor_WalkToAndWait(18, 0x118, 232);
    GameFlag_Set(0x200);
    Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a |= 1;
    Engine_EventEnd();
    L_02000916:;
}

void SceneEffect_ActivateNearbyActor(void)
{
    u8 *leader = Object_GetById(ACTOR_PARTY_LEADER);
    if ((*(s32 *)(leader + 16) >> 20) <= 13)
        Engine_ActorSetSpritePriority(20, 1);
}

void FieldScene_RunScene3a0_02000968(void)
{
    u32 i;
    s32 record;
    s32 v5;
    s32 x;

    Engine_EventBegin();
    *(u8 *)((u8 *)Object_GetById(20) + 35) &= 253;
    v5 = 0;
    *(u8 *)((u8 *)Object_GetById(20) + 85) = v5;
    record = Object_GetById(20);
    x = *(s32 *)(record + 8);
    record = Object_GetById(20);
    Map_CopyCellAttributes(3, 17, 1, 1, (x >> 20), (*(s32 *)(record + 16) >> 20));
    Engine_TaskAddCallback((s32)Actor_UpdatePresentationFlag, 0xc80);
    GameFlag_Set(0x201);
    Engine_ActorSetSpritePriority(20, 2);
    Engine_EventEnd();
}

void ShianMura_RunNpcMeetScene(void)
{
    s32 flag;
    s32 record;
    s32 cb;
    s32 back;
    s32 pos[3];
    struct SpawnParams params;

    Engine_EventBegin();
    flag = Engine_GameFlagIsSet(0x202);
    if (flag != 0) {
        Engine_ActorFaceActor(14, 0, 0);
        Engine_EventWait(10);
        Engine_EventSetMessage((s32)MsgShianKungFuStrong);
        Engine_EventShowMessage(14, 0);
        Engine_ActorFaceDirection(14, 0, 10);
        Engine_EventEnd();
    } else {
        Engine_EventSetMessage((s32)MsgShianHooWhaaaHachaaa);
        Engine_EventShowMessage(14, 0);
        Engine_ActorRunRepeatedMotion(0, 2);
        *(u8 *)((s32)Object_GetById(0) + 90) &= 254;
        pos[0] = flag;
        pos[1] = flag;
        pos[2] = flag;
        record = (s32)Object_GetById(0);
        Call3(Vector_AddPolarOffset, -0x80000, *(u16 *)(record + 6), (s32)pos);
        Engine_ActorSetAnimation(0, 2);
        Engine_ActorSetDestinationOffset(0, pos[0] / 0x10000, pos[2] / 0x10000);
        Engine_ActorWaitForMove(0);
        *(u8 *)((s32)Object_GetById(0) + 90) |= 1;
        Engine_EventWait(30);
        Engine_ActorRunRepeatedMotion(14, 2);
        params.count = 1;
        Effect_Spawn(0xc00000, 0, 0x1380000, 0x1999, 0x3333, 0, 0x20001, 0);
        Effect_Spawn(0xc00000, 0, 0x1380000, 0x3333, 0x1999, 0, 0x20001, 0);
        Engine_AudioPlayCue(132);
        cb = (s32)SceneEffect_SpawnPeriodicEffect;
        Value2(Engine_TaskAddCallback, cb, 0xc80);
        *(s32 *)((s32)Object_GetById(14) + 40) = 0x60000;
        *(s32 *)((s32)Object_GetById(14) + 72) = 0x10000;
        *(s32 *)((s32)Object_GetById(14) + 68) = 0;
        Call3(Engine_ActorSetSpeed, 14, 0x30000, 0x18000);
        Call3(Engine_ActorSetDestination, 14, 168, 0x138);
        Engine_ActorWaitForMove(14);
        Engine_AudioPlayCue(134);
        Engine_ActorEnableActionCallback(19, (s32)ShianMura_Actor19Motion);
        Engine_TaskAddCallback((s32)SceneEffect_AdvanceRotatingSprite, 0xc80);
        {
            s32 sprite = 0x11b;

            params.sprite = sprite;
        }
        params.callback = (s32)ShianMura_EffectScript;
        params.facing = 0x4000;
        Effect_Spawn(0xa80000, 0, 0x14c0000, 0, 0, 0, 0x720000, &params);
        Call3(Engine_ActorSetDestination, 14, 146, 0x138);
        Engine_ActorWaitForMove(14);
        ((void (*)())Engine_TaskRemoveCallback)(cb);
        Effect_Spawn(0x900000, 0, 0x1380000, 0, 0, 0, 0x20001, 0);
        Effect_Spawn(0x900000, 0, 0x1380000, -0x3333, 0x1999, 0, 0x20001, 0);
        back = -0x8000;
        Effect_Spawn(0x900000, 0, 0x1380000, back, 0, 0, 0x20001, 0);
        Object_RefreshSelectorById(19);
        Engine_AudioPlayCue(124);
        Effect_Spawn(0xa80000, 0x80000, 0x1380000, 0, 0, 0, 0x20001, 0);
        Effect_Spawn(0xa80000, 0x80000, 0x1380000, 0x3333, 0, 0, 0x20001, 0);
        Effect_Spawn(0xa80000, 0x80000, 0x1380000, -0x3333, 0, 0, 0x20001, 0);
        ((void (*)())Engine_TaskRemoveCallback)((s32)SceneEffect_AdvanceRotatingSprite);
        *(u16 *)(*(s32 *)((s32)Object_GetById(19) + 80) + 30) = back;
        *(s32 *)((s32)Object_GetById(14) + 68) = 0x4000;
        *(s32 *)((s32)Object_GetById(14) + 72) = 0x10000;
        Engine_EventWait(30);
        Engine_ActorFaceDirection(14, 0, 20);
        Engine_ActorRunRepeatedMotion(14, 2);
        Engine_EventWait(20);
        Engine_EventShowMessageAndWait(14, 0, 20);
        Engine_GameFlagSet(0x202);
        BattleFx_PlayQueuedSound();
        Engine_EventEnd();
    }
}

/*
 * The 28-byte owner includes its one pool word: 0x17f7 is an identifier
 * passed as an argument, not an address.  The first and last calls are the
 * scene bracket and must stay in that order.
 */
void SceneEffect_RunActorSceneMessage(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgShianWarriorsFromSchoolStrongWarriors);
    Engine_EventAskYesNo(17, 0);
    Engine_EventEnd();
}

/* Steps the leader half a cell ahead of the snapped cell it faces and
 * hops it there when nothing blocks the way. */
void ShianMura_HopLeaderAhead(void)
{
    s32 flags;
    struct FieldActor *leader;
    union FieldCoordinate pos[3];
    union FieldCoordinate *p;

    leader = (struct FieldActor *)Object_GetById(0);
    flags = leader->motion_flags;
    if (Engine_GameFlagIsSet(0x200) != 0) {
        p = pos;
        p[0].fixed = (leader->x.fixed & -0x100000) + 0x80000;
        p[1].fixed = leader->y.fixed;
        p[2].fixed = (leader->z.fixed & -0x100000) + 0x80000;
        Call3((void (*)())Vector_AddPolarOffset, 0x200000, (leader->facing + 0x2000) & 0xc000, (s32)p);
        if (((s32 (*)())Object_CheckMovementCollision)((s32)leader, (s32)p) == 0) {
            Engine_EventBegin();
            Object_SetMode(leader, 6);
            Engine_TaskWait(6);
            Engine_AudioPlayCue(152);
            Object_SetMode(leader, 7);
            leader->speed = 0x30000;
            leader->acceleration = 0x20000;
            leader->velocity_y = 0x40000;
            leader->motion_flags &= 126;
            Engine_ActorSetSpriteFlags(leader, 0);
            Engine_ObjectMotionSetPositionAndCommit(0, p[0].part.pixel, p[2].part.pixel);
            Object_SetMode(leader, 6);
            Engine_ActorSetSpriteFlags(leader, 1);
            leader->motion_flags = flags;
            Engine_EventEnd();
        }
    }
}

void FieldScene_RunScene3a0_02000de8(s32 a0)
{
    u32 i;
    s32 record;

    *(u8 *)((u8 *)Object_GetById(0) + 85) = 0;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    if (a0 == 6) {
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
        Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
    } else {
        Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
    }
    gEventWork->transition_frames = 16;
    Engine_EventRequestExit(a0);
}

/*
 * The selector is halfword [182] of the event work,
 * guarded to the range 1..7.  The pointer is loaded before the first call and
 * held across all of them, so it is a function-top local.  Cases 2 and 3 set
 * the two shared arguments and jump into the middle of case 6 to share its
 * final call; the goto and the two locals are what reproduce that.
 */
void SceneEffect_DispatchStep(void)
{
    s16 *scene = (s16 *)gEventWork;
    u8 *shared0;
    s32 shared1;

    Engine_EventBegin();

    switch (scene[182]) {
    case 1:
        Audio_PlayCue(158);
        Map_AnimateCells(ShianMura_GateSteps1, 81, 18);
        break;
    case 2:
        Audio_PlayCue(158);
        shared0 = ShianMura_GateSteps2;
        shared1 = 83;
        goto shared;
    case 3:
        Audio_PlayCue(158);
        shared0 = ShianMura_GateSteps2;
        shared1 = 86;
        goto shared;
    case 4:
        Audio_PlayCue(158);
        Map_AnimateCells(ShianMura_GateSteps3, 84, 24);
        break;
    case 5:
        Audio_PlayCue(158);
        Map_AnimateCells(ShianMura_GateSteps3, 72, 7);
        break;
    case 6:
        Audio_PlayCue(188);
        shared0 = ShianMura_GateSteps4;
        shared1 = 69;
    shared:
        Map_AnimateCells(shared0, shared1, 11);
        break;
    case 7:
        Audio_PlayCue(158);
        Map_AnimateCells(ShianMura_GateSteps5, 83, 7);
        break;
    default:
        break;
    }

    FieldScene_RunScene3a0_02000de8(scene[182]);
    Engine_EventEnd();
}

void Scene_RunActorNineteenScript(void)
{
    s32 cb;

    Engine_EventBegin();
    ((s32 (*)())Engine_ActorEnableActionCallback)(19, (s32)ShianMura_Actor19Motion);
    cb = (s32)SceneEffect_AdvanceRotatingSprite;
    ((s32 (*)())Engine_TaskAddCallback)(cb, 0xc80);
    Object_RefreshSelectorById(19);
    Engine_AudioPlayCue(124);
    Effect_Spawn(0xa80000, 0x80000, 0x1380000, 0, 0, 0, 0x20001, 0);
    Effect_Spawn(0xa80000, 0x80000, 0x1380000, 0x3333, 0, 0, 0x20001, 0);
    Effect_Spawn(0xa80000, 0x80000, 0x1380000, -0x3333, 0, 0, 0x20001, 0);
    Engine_TaskRemoveCallback(cb);
    Object_GetById(19)->sprite->rotation = 0x8000;
    Engine_ActorSetPosition(21, 0xa80000, 0x1380000);
    Engine_EventWait(20);
    Engine_ActorFaceActor(14, 19, 0);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(10);
    Engine_EventSetMessage((s32)MsgShianWhaWhaHappened);
    if (Value1((s32 (*)())Engine_GameFlagIsSet, 0x203)) {
        gEventWork->message++;
    }
    Engine_EventShowMessage(14, 0);
    Engine_GameFlagSet(0x203);
    BattleFx_PlayQueuedSound();
    Engine_EventEnd();
}

void FieldScene_RunScene3a0_02001060(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    ((void (*)())Engine_ActorEnableActionCallback)(18, 1);
    record = Actor_Get(18);
    *(s32 *)(record + 108) = 0;
    record = Actor_Get(18);
    *(s32 *)(record + 56) = -0x80000000;
    record = Object_GetById(18);
    *(s32 *)(record + 64) = -0x80000000;
    record = Object_GetById(18);
    *(s32 *)(record + 36) = 0;
    record = Object_GetById(18);
    *(s32 *)(record + 44) = 0;
    record = Object_GetById(18);
    *(s32 *)(record + 48) = 0;
    record = Actor_Get(18);
    *(s32 *)(record + 52) = 0;
    Actor_ShowEmote(18, 0x103, 0);
    Engine_ActorStartRepeatedMotion(18, 2);
    Engine_EventWait(60);
    Actor_SetSpeed(18, 0x18000, 0xc000);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
    Actor_WalkTo(18, 0x118, 232);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x128, 232);
    Engine_ActorWaitForMove(18);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
    Engine_ActorEnableActionCallback(18, ShianMura_ActionTable);
    record = Actor_Get(18);
    *(s32 *)(record + 108) = (s32)ShianMura_WatchGateTrigger;
    Engine_EventEnd();
}

u8 *SceneEffect_GetTertiaryData(void)
{
    return gShianMuraEvents;
}

s32 ShianMura_SetupScene(void)
{
    struct FieldActor *actor;
    u32 n;
    s32 x;

    gEventWork->start_transition = 0x100;
    BattleFx_SetQueuedSoundAndPlay(169);
    if (gGameState.entrance > 9) {
        Engine_GameFlagClear(0x12f);
    }
    if (Engine_GameFlagIsSet(0x895)) {
        Call3((void (*)())Engine_ActorFaceDirection, 13, 0x8000, 0);
        Call3((void (*)())Engine_ActorSetPosition, 14, 0x920000, 0x1380000);
        Engine_ActorFaceDirection(14, 0, 0);
        if (Engine_GameFlagIsSet(0x89a)) {
            Engine_ActorSetPosition(17, 0, 0);
        }
    }
    if (Engine_GameFlagIsSet(0x8b0)) {
        Engine_ActorSetPosition(17, 0, 0);
    }
    for (n = 0; n <= 2; n++) {
        struct FieldActor *actor;
        actor = Object_GetById(n + 23);
        /* FAKEMATCH: one value spans priority and motion setup so the
         * motion zero stays inside the loop and feeds the later actors. */
        x = 1;
        actor->sprite->priority = x;
        x = 0;
        actor->motion_flags = x;
        actor->collision_flags = 8;
        Engine_ActorSetSpriteFlags(actor, 0);
        ObjectGroup_SetChildValue(actor, 15);
        actor->priority_flags = (actor->priority_flags & 254) | 2;
    }
    if (Engine_GameFlagIsSet(0x202)) {
        Call3((void (*)())Engine_ActorSetPosition, 14, 0x920000, 0x1380000);
        Engine_ActorFaceDirection(14, 0, 0);
    }
    if (Engine_GameFlagIsSet(0x201)) {
        Engine_ActorSetAnimation(20, 5);
        {
            s32 px = Object_GetById(20)->x.fixed;

            Engine_MapCopyCellAttributes(3, 17, 1, 1, px >> 20, Object_GetById(20)->z.fixed >> 20);
        }
        ((void (*)())Engine_TaskAddCallback)((s32)Actor_UpdatePresentationFlag, 0xc80);
    }
    Engine_ActorSetChildValue(18, 2);
    Object_GetById(18)->update = ShianMura_WatchGateTrigger;
    actor = Object_GetById(19);
    actor->motion_flags = x;
    actor->y.fixed = 0x100000;
    actor->target_y = 0x100000;
    actor->scale_x = 0x8ccc;
    actor->scale_y = 0x6666;
    actor->sprite->rotation = 0x8000;
    Engine_ActorSetSpriteFlags(Object_GetById(21), 0);
    {
        /* FAKEMATCH: narrow local retains the short-range zero pool load. */
        u8 shown = 0;

        Object_GetById(21)->motion_flags = shown;
    }
    Object_GetById(21)->y.fixed = x;
    Object_GetById(21)->target_y = -0x80000000;
    return 0;
}
