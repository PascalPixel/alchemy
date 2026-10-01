/* NONMATCHING: 2026-10-01 brief Wave2 one-device attempt.
 * Effect_AdvanceMotion: removing the do-once at source line 455 changes
 * first changed instruction: str r3, [r6, #12] => ldr r7, [r6, #76]; 47/47 assembly lines.
 * The production source retains and tags this scheduling boundary.
 * Other functions in this unit are unchanged from the current source.
 */
#include "../../../../../../../games/THE BROKEN SEAL/SRC/FIELD/VINASU_HEYA/ENTRY_SETUP.H"

/* Preserve the original measured FIELD_EVENT adapter context of this draft. */
static inline void Event_Begin(void)
{
    Engine_EventBegin();
}

static inline void Event_End(void)
{
    Engine_EventEnd();
}

static inline void Event_Wait(s32 frames)
{
    Engine_EventWait(frames);
}

static inline void Task_Wait(s32 frames)
{
    Engine_TaskWait(frames);
}

static inline void Event_SetMessage(s32 message)
{
    Engine_EventSetMessage(message);
}

static inline void Message_ShowCentered(s32 message, s32 flags)
{
    Engine_MessageShowCentered(message, flags);
}

static inline void Event_RequestExit(s32 exit)
{
    Engine_EventRequestExit(exit);
}

static inline void Event_OpenScreen(void)
{
    Engine_EventOpenScreen();
}

static inline void Event_CloseScreen(void)
{
    Engine_EventCloseScreen();
}

static inline void Event_WaitForScreen(void)
{
    Engine_EventWaitForScreen();
}

static inline void Actor_WaitForMove(s32 actor)
{
    Engine_ActorWaitForMove(actor);
}

static inline void Actor_FaceEachOther(s32 actor, s32 other, s32 frames)
{
    Engine_ActorFaceEachOther(actor, other, frames);
}

static inline void Actor_SetAnimation(s32 actor, s32 animation)
{
    Engine_ActorSetAnimation(actor, animation);
}

static inline void Actor_SetAnimationAndWait(s32 actor, s32 animation)
{
    Engine_ActorSetAnimationAndWait(actor, animation);
}

static inline void Actor_RunRepeatedMotion(s32 actor, s32 repeats)
{
    Engine_ActorRunRepeatedMotion(actor, repeats);
}

static inline void Actor_SetSpriteFlags(struct FieldActor *actor, s32 flags)
{
    Engine_ActorSetSpriteFlags(actor, flags);
}

static inline void Camera_WaitForMove(void)
{
    Engine_CameraWaitForMove();
}

static inline void Map_Redraw(void)
{
    Engine_MapRedraw();
}

static inline s32 Math_Sin(s32 angle)
{
    return Engine_MathSin(angle);
}

static inline s32 Math_Cos(s32 angle)
{
    return Engine_MathCos(angle);
}

static inline void Object_SetAnimation(struct FieldActor *object, s32 animation)
{
    Object_SetMode(object, animation);
}

static inline void Object_SetPalette(struct FieldActor *object, s32 palette)
{
    ObjectGroup_SetChildValue(object, palette);
}

static inline void Actor_EnableActionCallback(s32 actor, const u8 *table)
{
    Engine_ActorEnableActionCallback(actor, table);
}

static inline void ColorBuffer_Interpolate(s32 frames)
{
    Engine_ColorBufferInterpolate(frames);
}

static inline void MapRender_WaitForValues(void)
{
    Engine_MapRenderWaitForValues();
}

#include "IWRAM_CALL.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"

extern u8 *gActorEffectWork;

extern const struct SceneEntrance gVinasuHeyaEntrances1[];
extern const struct SceneEntrance gVinasuHeyaEntrances3[];
extern const struct SceneEntrance gVinasuHeyaEntrances4[];
extern const struct SceneEntrance gVinasuHeyaEntrances5[];
extern const struct SceneEntrance gVinasuHeyaEntrances6[];
extern const struct SceneEntrance gVinasuHeyaEntrancesOther[];

extern u8 gVinasuHeyaPlacements1[];
extern u8 gVinasuHeyaPlacements2[];
extern u8 gVinasuHeyaPlacements3[];
extern u8 gVinasuHeyaPlacements4[];
extern u8 gVinasuHeyaPlacements5[];
extern u8 gVinasuHeyaPlacements6[];
extern u8 gVinasuHeyaPlacementsOther[];
void FieldScene_PrepareActors(u8 *placements);

extern u8 gVinasuLeaderApproachScript[];
s32 SceneEffect_SpawnRandomEveryEightFramesB();
extern u8 MsgFieldDoorTightlyLocked[];
extern u8 MsgFieldVenusLighthouseWasAttackedBy[];
extern u8 MsgVinasuHmmmWeCantPushBlock[];
extern u8 MsgVinasuIveWaitedLongSeeIts[];
extern u8 MsgVinasuStatueSpeaksRobinSoulYe[];
extern u8 MsgVinasuThereWordsCarvedIntoRelief[];

extern u8 MsgVinasuThoughtIdExploreAfterDoor[];

void OverlayObject_WaitUntilIdle();
void Engine_ActorSetSpriteFlags();
void Engine_EventBegin();
void Engine_EventWait();
void Engine_ActorSetSpeed();
void Engine_MapCopyCellAttributes();
void Engine_ActorMoveToAndWait();
void Engine_ActorSetAnimation();
void Engine_EventEnd();
void Engine_ActorRunRepeatedMotion();
void Object_SetActionById();
void Engine_CameraMoveTo();
void Engine_ActorSetAttachedEffect();
void Engine_ActorSetChildValue();
void Engine_EventRequestExit();
void Engine_EventCloseScreen();
void Engine_EventWaitForScreen();

s32 SceneActor_CalculateFixedPointDistance(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 dxsq = dx *dx;
    s32 dysq = dy *dy;
    s32 dzsq = dz *dz;

    return Iwram_Sqrt(dxsq + dysq + dzsq);
}

/*
 * CALL SYMBOLS ARE PER-SITE: the raw assembly spells each of these eight
 * calls as a direct `bl sub_020072xx` to an address inside this overlay's
 * own 0x0200xxxx range (verified via `arm-none-eabi-objdump -dr -M
 * force-thumb` on the assembled .o, which resolves the *ABS* targets before
 * linking) -- lifted verbatim, not the veneer-math final target names this
 * file used before. The one true indirect call (selector's local-effect
 * dispatch) is routed automatically through this overlay's own
 * `_call_via_rN` bank; SceneEffect_SpawnNineRadialEffects is correct for it, unchanged.
 */
s32 *SceneData_FindSlotAtPosition(pos)
    s32 *pos;
{
    extern u8 *gWork;

    s32 **slots = (s32 **)(gWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if ((pos[0] >> 20) == (p[2] >> 20)
            && (pos[1] >> 20) == (p[3] >> 20)
            && (pos[2] >> 20) == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

void RunStagedActorTransition(void)
{
    s32 target_position[3];
    struct StagedActor *leader;
    struct StagedActor *actor;
    struct StagedActor *blocking_actor;
    s32 direction_index;
    u32 packed_step;
    s32 move_rate;
    s32 transition_busy;

    leader = Actor_Get(ACTOR_PARTY_LEADER);
    direction_index = leader->direction_and_kind >> 12;
    packed_step = StagedActor_DirectionSteps[direction_index];
    target_position[0] = leader->x.value + (packed_step & 0xffff0000);
    target_position[1] = leader->y;
    packed_step <<= 16;
    target_position[2] = leader->z.value + packed_step;
    actor = SceneData_FindSlotAtPosition(target_position, leader);
    if (actor == 0) return;

    packed_step = StagedActor_DirectionSteps[direction_index];
    target_position[0] = actor->x.value + (packed_step & 0xffff0000);
    target_position[1] = actor->y;
    packed_step <<= 16;
    target_position[2] = actor->z.value + packed_step;
    blocking_actor = SceneData_FindSlotAtPosition(target_position, actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    target_position[0] = actor->x.value;
    target_position[1] = actor->y + 0x100000;
    target_position[2] = actor->z.value;
    blocking_actor = SceneData_FindSlotAtPosition(target_position, actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    actor->transition_mode = 2;
    packed_step = StagedActor_DirectionSteps[direction_index];
    target_position[0] = actor->x.value + (packed_step & 0xffff0000);
    target_position[1] = actor->y;
    packed_step <<= 16;
    target_position[2] = actor->z.value + packed_step;
    if (Object_CheckMovementCollision(actor, target_position) > 0) return;

    transition_busy = actor->transition_busy;
    if (transition_busy != 0) return;

    Object_SetAnimation(leader, 8);
    move_rate = 0x3333;
    Task_Wait(15);
    Audio_PlayCue(185);
    actor->move_rate_x = move_rate;
    actor->move_rate_z = move_rate;
    Engine_ObjectSetPosition(actor, target_position[0], target_position[1], target_position[2]);
    leader->move_rate_x = move_rate;
    leader->move_rate_z = move_rate;
    Engine_ObjectSetPosition(leader, target_position[0], target_position[1], target_position[2]);
    Engine_ObjectCommitPosition(actor);
    BattleFx_PlayQueuedSound();
    actor->x.value = target_position[0];
    actor->z.value = target_position[2];
    actor->unknown_24 = transition_busy;
    actor->unknown_2c = transition_busy;
    leader->unknown_38 = 0x80000000;
    leader->unknown_40 = 0x80000000;
    leader->unknown_24 = transition_busy;
    leader->unknown_2c = transition_busy;
    leader->x.value = leader->x.parts.cell << 16;
    leader->z.value = leader->z.parts.cell << 16;
    Object_SetAnimation(leader, 1);
}

s32 SceneState_FillGridCellByte2(u32 no, s32 x, s32 y, u32 w, u32 h, s32 val)
{
    u8 *g = gMapWork;
    u8 *base;
    u32 i;
    u32 j;

    if (g != 0) {
        if (no <= 2) {
            u32 off = no * 48 + 304;

            base = *(u8 **)(g + off);
        } else {
            base = gMapCellBuffer;
        }
        base += (x + (y << 7)) * 4;
        for (i = 0; i < h; i++) {
            u8 *p = base + (i << 9);

            for (j = 0; j < w; j++) {
                p[2] = (u8)val;
                p += 4;
            }
        }
    }
    return 0;
}

/* Named shorthand for one fixed argument pair, in overlay resource_3c8. */
void SceneActor_ApplySlotsMatchingKind212(void)
{
    extern u8 *gWork;

    s32 **slots = (s32 **)(gWork + 0x14);
    u32 i;
    s32 lim = 0x212;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];
        u32 h = *(u16 *)((u8 *)p + 100);
        s32 t = h << 16;

        if ((t >> 20) == lim) {
            s32 m = 15;
            m &= h;
            Object_SetPalette(p, m);
        }
    }
}

s32 OverlayObject_ApplyLowNibbleOfField100(void *obj)
{
    Object_SetPalette(obj, *(u16 *)((u8 *)obj + 100) & 15);
    return 0;
}

s32 OverlayObject_UpdateEveryFourFrames(void *obj)
{
    if ((*(u32 *)&gFrameCount & 3) == 0)
        Object_SetPalette(obj, 7);
    else
        Object_SetPalette(obj, 0);

    if ((*(u32 *)&gFrameCount & 7) == 0)
        Audio_PlayCue(138);
    return 0;
}

s32 SceneEffect_SpawnRandomEveryEightFramesB(struct Object_020005e4 *object)
{
    struct EffectParams_020005e4 params;
    s32 phase, x, y, speed;
    phase = *(u32 *)&gFrameCount & 7;
    if (phase != 0) goto done;
    params.unk00 = 3 - (s32)((u32)(Random_Next() * 2) >> 16);
    params.color1 = 0x6666;
    params.color2 = 0x6666;
    params.mode = 14;
    x = object->x + (((s32)((u32)(Random_Next() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - (s32)((u32)(Random_Next() * 32) >> 16)) << 16);
    speed = Math_Divide(((s32)((u32)(Random_Next() * 5) >> 16) << 16) + 0x00050000, 10);
    Effect_Spawn(x, y, object->z, 0, speed, phase, 0x000b0000, &params);
done:
    return 0;
}

s32 OverlayObject_ApplyZero(void *obj)
{
    Actor_SetSpriteFlags(obj, 0);
    return 0;
}

s32 SceneEffect_SpawnTwoRandomizedParticles(struct Object_020006a0 *obj)
{
    struct EffectParams_020006a0 params;
    s32 speed;
    s32 phase;

    params.color1 = 0x0000cccc;
    params.color2 = 0x0000cccc;
    params.unk00 = 0;

    speed = (s32)((u32)(Random_Next() * 8) >> 16) * 0x3333;
    phase = gFrameCount & 15;

    Effect_Spawn(
        obj->x + ((8 - phase) << 16),
        obj->y + 0x001a0000,
        obj->z,
        0,
        -speed,
        0,
        0x000a0000,
        &params);

    phase = gFrameCount & 15;
    if (phase == 0) {
        params.color1 = 0x00008000;
        params.color2 = 0x00008000;
        Effect_Spawn(
            obj->x
                + (((s32)((u32)(Random_Next() * 9) >> 16) - 4) << 16),
            obj->y,
            obj->z,
            0,
            0,
            0,
            0x000a0000,
            &params);
    }

    return 0;
}

void SceneEffect_RunObjectZeroColorSequence(void)
{
    struct EffectObject *obj;
    u8 *state;

    state = *(u8 **)&gEventWork;
    obj = Actor_Get(ACTOR_PARTY_LEADER);
    Event_Begin();
    Audio_PlayCue(228);
    obj->callback = (s32)SceneEffect_SpawnTwoRandomizedParticles;
    obj->color = 0x3333;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    obj->callback = 0;
    Event_Wait(30);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(*(s16 *)(state + 0x16c));
    Event_End();
}

/* Runs a guarded one-shot setup on an entry record: only fires the first
 * time (while a global 0x109 lookup is still unset), positions the entry
 * from its own stored coordinates, drives an effect/param sequence, then
 * writes a stage byte and an override field on the entry before returning. */
void FieldScene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    u8 *entry;
    u8 *guard;
    u8 *sub;

    entry = Actor_Get(ACTOR_PARTY_LEADER);
    guard = GameFlag_IsSet(0x109);
    if (guard == 0) {
        Event_Begin();
        Camera_MoveTo(-1, -1, -1, 0);
        /* Stage byte at +85 of the entry record. */
        entry[85] = guard;
        /* Position, from the entry's own s16 coordinates at +10/+18
         * (converted to 16.16 fixed point; the y term is offset by -16.0). */
        Actor_SetPosition(ACTOR_PARTY_LEADER, (*(s16 *)(entry + 10) << 16), ((*(s16 *)(entry + 18) << 16) + -0x100000));
        Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
        sub = Actor_Get(ACTOR_PARTY_LEADER);
        Actor_SetSpriteFlags(sub, 0);
        Event_OpenScreen();
        Event_WaitForScreen();
        Audio_PlayCue(228);
        /* Override field at +108 of the entry record; holds an EWRAM
         * address while the effect sequence below runs. */
        *(s32 *)(entry + 108) = (s32)SceneEffect_SpawnTwoRandomizedParticles;
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
        Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
        Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
        sub = Actor_Get(ACTOR_PARTY_LEADER);
        Actor_SetSpriteFlags(sub, 1);
        Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
        entry[85] = 3;
        /* Restore the +108 override field to the original (unset) value. */
        *(s32 *)(entry + 108) = guard;
        BattleFx_PlayQueuedSound();
        Event_End();
    }
}

void SceneState_StoreLookupZeroToWord24(void)
{
    *(s32 *)(gActorEffectWork + 24) = Actor_Get(ACTOR_PARTY_LEADER);
}

void SceneState_ClearWorkspaceWord24(void)
{
    *(s32 *)(gActorEffectWork + 24) = 0;
}

s32 SceneActor_SetFlagBitByRelativeDepth(struct Actor_020008c8 *actor)
{
    struct Actor_020008c8 *ref;
    u8 *fp;
    u8 flags;
    ref = Actor_Get(ACTOR_PARTY_LEADER);
    fp = &actor->flatla3;
    flags = *fp | 2;
    *fp = flags;
    if (ref->z < actor->z) {
        s32 diff = actor->z - ref->z;
        s32 lim;
        diff += 0x00040000;
        lim = actor->y;
        lim += diff;
        if (ref->y <= lim) {
            flags &= 0xfd;
            *fp = flags;
        }
    }
    return 0;
}

void SceneActor_SwapPositionsByDepth(s32 group, s32 index)
{
    struct Position *first;
    struct Position *second;
    s32 value;

    first = Actor_Get(group);
    second = Actor_Get(index);
    if (first->z <= second->z) {
        value = first->x;
        first->x = second->x;
        second->x = value;

        value = first->y;
        first->y = second->y;
        second->y = value;

        value = first->z;
        first->z = second->z;
        second->z = value;
        Task_Wait(1);
    }
}

void OverlayObject_WaitUntilIdle(struct BusyObject *obj)
{
    s32 cnt;
    s32 busy;

    cnt = 60;
    while (cnt != 0) {
        Task_Wait(1);
        busy = obj->busy;
        cnt--;
        if (busy == 0) break;
    }
}

/*
 * Owner at 0x0200096c.  Add the velocity at +68/+72/+76 into the position at
 * +8/+12/+16, decay the X and Z velocities, accumulate the rates at +48/+52
 * into +24/+28, and advance the sprite angle by the record's step.  The Z
 * decay must stay written as a signed divide by 16 -- the negative bias and
 * arithmetic shift are what that division compiles to.
 */
void Effect_AdvanceMotion(struct Effect_0200096c *effect)
{
    s32 velocity_z;
    struct Sprite_0200096c *sprite;
    s32 velocity_x;

    /* The block keeps the Z load after the Y store. */
    
    velocity_x = effect->velocity[0];
    effect->position[0] += velocity_x;
    effect->position[1] += effect->velocity[1];

    velocity_z = effect->velocity[2];
    effect->position[2] += velocity_z;

    effect->velocity[0] = velocity_x - Math_Divide(velocity_x, 18);
    effect->velocity[2] = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    struct FieldActor *actor;
    u32 i;
    struct EffectOptions options;

    actor = Actor_Get(ACTOR_PARTY_LEADER);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Map_Redraw();
    Task_Wait(1);
    actor->y.fixed = 0x820000;
    *(s32 *)((u8 *)actor + 72) = 0x8000;
    *(s32 *)((u8 *)actor + 68) = 0;
    actor->motion_flags = 0;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(30);
    Audio_PlayCue(204);
    actor->motion_flags = 3;
    Event_Wait(24);
    options.palette = 7;
    options.update = (void (*)(union FieldObject *))Effect_AdvanceMotion;
    options.start_scale_x = 0xcccc;
    options.start_scale_y = 0xcccc;
    for (i = 0; i <= 16; i++) {
        s32 angle;
        s32 velocity[3];

        angle = i << 12;
        velocity[0] = Math_Cos(angle);
        velocity[1] = 0;
        velocity[2] = Math_Sin(angle);
        velocity[0] += velocity[0] / 2;
        Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, velocity[0], velocity[1],
                      velocity[2], 0x1090001, &options);
    }
    Audio_PlayCue(188);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 22);
    Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x100);
    BattleFx_PlayQueuedSound();
    *(s32 *)((u8 *)actor + 72) = 0x10000;
    *(s32 *)((u8 *)actor + 68) = 0x4000;
    Event_End();
}

void SceneEffect_SpawnNineRadialEffects(s32 actor)
{
    struct SceneObject *object;
    struct Vec vec;
    struct EffectParams params;
    u32 i;
    s32 v;
    s32 x;
    s32 z;

    object = (struct SceneObject *)Object_GetById(actor);
    params.unk00 = 1;
    params.mode = 7;
    params.callback = (s32)Effect_AdvanceMotion;
    for (i = 0; i <= 16; i += 2) {
        v = i << 12;
        vec.x = Math_Cos(v);
        vec.y = 0;
        z = Math_Sin(v);
        x = vec.x;
        vec.z = z;
        x = x + Math_Divide(x, 3);
        vec.x = x;
        Effect_Spawn(object->x, object->y, object->z, x, vec.y, z, 0x01030001, &params);
    }
}

s32 SceneEffect_SpawnRandomizedParticleEveryFourFrames(struct SceneObject_02000b98 *obj)
{
    struct EffectParams_02000b98 params;
    s32 y;
    s32 a;
    s32 b;
    s32 rnd;

    if ((gFrameCount & 3) != 0) {
        return 0;
    }
    if ((u32)(Random_Next() * 6) >> 16 == 0) {
        if (obj->near != 0x80000000 || obj->far != 0x80000000) {
            Audio_PlayCue(246);
        }
    }
    y = 0;
    params.angle = 286;
    params.color1 = 0x10000;
    params.color2 = 0x10000;
    params.unk10 = -327;
    params.unk14 = -327;
    rnd = Random_Next();
    a = Math_Divide(((((u32)(rnd * 9)) >> 16) - 4) << 16, 10);
    rnd = Random_Next();
    b = Math_Divide(((((u32)(rnd * 9)) >> 16) - 4) << 16, 10);
    Effect_Spawn(obj->x, obj->y, obj->z - 0x10000, a, y, b, 0x001c0001, &params);
    return 0;
}

struct EffectObject_02000c5c *SceneEffect_SpawnEffect284AtCell(s32 x, s32 z, s32 arg2)
{
    struct EffectObject_02000c5c *obj;
    s32 sx;
    s32 sz;

    sx = x << 16;
    sz = z << 16;
    obj = Object_Create(284, sx, 0, sz);
    if (obj == 0) {
        return 0;
    }
    obj->scale_x = 0x10000;
    obj->scale_y = 0x10000;
    Actor_SetSpriteFlags(obj, 0);
    Object_SetAnimation(obj, 7);
    obj->state = 0;
    obj->timer = 0;
    obj->phase = 0;
    obj->mode = 2;
    obj->callback = (s32)SceneEffect_SpawnRandomizedParticleEveryFourFrames;
    obj->flag = 0;
    Object_SetScript(obj, arg2);
    return obj;
}

s32 SceneActor_TryMoveActorZeroTwoTilesAhead(void)
{
    struct SceneObject_02000cc8 *obj;
    struct Vec vec;
    u8 *state;
    u8 old;
    s32 m;

    obj = Actor_Get(ACTOR_PARTY_LEADER);
    state = &obj->state;
    old = *state;
    vec.x = (obj->x & 0xfff00000) + 0x80000;
    vec.y = obj->y;
    vec.z = (obj->z & 0xfff00000) + 0x80000;
    m = (obj->angle + 0x2000) & 0xc000;
    Vector_AddPolarOffset(0x100000, m, &vec);
    if (Object_CheckMovementCollision(obj, &vec) != 1 && SceneData_FindSlotAtPosition(&vec, obj) == 0) {
        vec.x = (obj->x & 0xfff00000) + 0x80000;
        vec.y = obj->y;
        vec.z = (obj->z & 0xfff00000) + 0x80000;
        Vector_AddPolarOffset(0x200000, (obj->angle + 0x2000) & 0xc000, &vec);
        if (SceneData_FindSlotAtPosition(&vec, obj) == 0 && Object_CheckMovementCollision(obj, &vec) == 0) {
            Event_Begin();
            Object_SetAnimation(obj, 6);
            Task_Wait(6);
            Audio_PlayCue(152);
            Object_SetAnimation(obj, 7);
            obj->scale_x = 0x30000;
            obj->scale_y = 0x20000;
            obj->accel = 0x40000;
            *state &= 0x7e;
            Actor_SetSpriteFlags(obj, 0);
            Actor_MoveToAndWait(ACTOR_PARTY_LEADER, ((union VecView *)&vec)->h[1], ((union VecView *)&vec)->h[5]);
            Object_SetAnimation(obj, 6);
            Actor_SetSpriteFlags(obj, 1);
            *state = old;
            Event_End();
            return 1;
        }
    }
    return 0;
}

/* Where the party appears in each of the rooms; the second takes the table the other scenes take. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&SceneId_VinasuHeya1) {
        return gVinasuHeyaEntrances1;
    }
    if (v == (s32)&SceneId_VinasuHeya3) {
        return gVinasuHeyaEntrances3;
    }
    if (v == (s32)&SceneId_VinasuHeya4) {
        return gVinasuHeyaEntrances4;
    }
    if (v == (s32)&SceneId_VinasuHeya5) {
        return gVinasuHeyaEntrances5;
    }
    if (v == (s32)&SceneId_VinasuHeya6) {
        return gVinasuHeyaEntrances6;
    }
    return gVinasuHeyaEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetPrimaryTable(void)
{
    return gVinasuHeyaPrimaryTable;
}

/* The actors placed in each room. Every room but the first has its table
   prepared before it is returned. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    u8 *ret;
    s16 v;

    v = gGameState.scene;
    if (v == (s32)&SceneId_VinasuHeya1) {
        return (const struct ScenePlacement *)gVinasuHeyaPlacements1;
    }
    if (v == (s32)&SceneId_VinasuHeya2) {
        ret = gVinasuHeyaPlacements2;
    } else if (v == (s32)&SceneId_VinasuHeya3) {
        ret = gVinasuHeyaPlacements3;
    } else if (v == (s32)&SceneId_VinasuHeya4) {
        ret = gVinasuHeyaPlacements4;
    } else if (v == (s32)&SceneId_VinasuHeya5) {
        ret = gVinasuHeyaPlacements5;
    } else if (v == (s32)&SceneId_VinasuHeya6) {
        ret = gVinasuHeyaPlacements6;
    } else {
        goto no_match;
    }
    FieldScene_PrepareActors(ret);
    return (const struct ScenePlacement *)ret;

no_match:
    return (const struct ScenePlacement *)gVinasuHeyaPlacementsOther;
}

void SceneState_SetFlag953(void)
{
    Event_Begin();
    Message_ShowCentered((s32)MsgFieldDoorTightlyLocked, 1);
    Event_End();
}

void FieldScene_RunActorEightTenStepLoop(void)
{
    u32 n;
    u32 w;
    s32 a;
    s32 b;

    Event_Begin();
    Actor_RunRepeatedMotion(8, 3);
    Event_SetMessage((s32)MsgFieldVenusLighthouseWasAttackedBy);
    n = 10;
    w = 8;
    Event_ShowMessageAndWait(8, 0, 20);
    do {
        Actor_SetChildValue(8, 15);
        Task_Wait(2);
        Actor_SetChildValue(8, 0);
        Task_Wait(w);
        if (w > 3) {
            w--;
        }
        n--;
    } while (n != 0);
    GameFlag_Set(0x981);
    Actor_SetPosition(8, 0, 0);
    a = 7;
    b = 16;
    Map_CopyCellAttributes(7, 17, 2, 1, a, b);
    Event_End();
}

void SceneDialogue_RunActorElevenDialogue(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgVinasuIveWaitedLongSeeIts);
    Event_ShowMessageAndWait(11, 0, 20);
    Actor_RunRepeatedMotion(11, 2);
    Event_ShowMessage(11, 0);
    Event_End();
}

void FieldScene_SetFlag987AtActorTwelveTile(void)
{
    Struct_0ff0 *s;

    s = Actor_Get(12);
    Event_Begin();
    if (s->unk8 >> 20 == 54 || s->unk10 >> 20 == 6) {
        GameFlag_Set(0x987);
    }
    Event_End();
}

#if defined(TBS_EDITION_EN)

/* Only the English edition reads the words carved into the relief. */
void SceneDialogue_ReadRelief(void)
{
    Event_Begin();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Message_ShowCentered((s32)MsgVinasuThereWordsCarvedIntoRelief, 1);
    Event_End();
}

#endif

void SceneState_ApplySixRectsAfter161(void)
{
    s32 x;
    s32 y;
    s32 a;
    s32 b;

    GameFlag_Clear(0x161);
    x = 23;
    y = 8;
    Map_CopyCellAttributes(35, 8, 1, 3, x, y);
    a = 3;
    b = 1;
    Map_CopyCellsTo(35, 8, 23, 8, b, a);
    Map_CopyCellsTo(99, 8, 87, 8, b, a);
    x = 46;
    y = 55;
    Map_CopyCellAttributes(57, 55, 3, 3, x, y);
    Map_CopyCellsTo(57, 55, 46, 55, a, a);
    Map_CopyCellsTo(121, 55, 110, 55, a, a);
}

void SceneState_ApplySixRectsAfterFlag161(void)
{
    s32 x;
    s32 y;
    s32 a;
    s32 b;

    GameFlag_Set(0x161);
    x = 23;
    y = 8;
    Map_CopyCellAttributes(36, 8, 1, 3, x, y);
    a = 3;
    b = 1;
    Map_CopyCellsTo(36, 8, 23, 8, b, a);
    Map_CopyCellsTo(100, 8, 87, 8, b, a);
    x = 46;
    y = 55;
    Map_CopyCellAttributes(53, 55, 3, 3, x, y);
    Map_CopyCellsTo(53, 55, 46, 55, a, a);
    Map_CopyCellsTo(117, 55, 110, 55, a, a);
}

void FieldScene_RunLeaderSurpriseApproach(void)
{
    struct FieldActor *actor;
    s32 z;

    actor = Actor_Get(ACTOR_PARTY_LEADER);
    Event_Begin();
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, gVinasuLeaderApproachScript);
    Engine_ActorStartAction(0);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 6);
    actor->velocity_y = 0x40000;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x40000, 0x20000);
    if (actor->z.fixed >> 20 <= 54) {
        Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a &= 254;
        z = 210;
    } else {
        Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a &= 254;
        z = 238;
    }
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, actor->x.part.pixel, z << 2);
    Event_Wait(1);
    SetFlagBits(&Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a, 1);
    Event_Wait(20);
    actor->update = (void (*)(union FieldObject *))SceneEffect_SpawnRandomEveryEightFramesB;
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
    actor->update = 0;
    Event_End();
}

void FieldScene_RunStatueDialogueSequence(void)
{
    extern struct EventWorkState *gWork;
    struct EventWorkState *work;

    work = gWork;
    work->field_cba = 0;
    work->field_cb6 = 1;
    Event_Begin();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Message_ShowCentered((s32)MsgVinasuStatueSpeaksRobinSoulYe, 1);
    ColorBuffer_ApplySource(0x10000, 0);
    ColorBuffer_ApplyTarget(0x10005, 0);
    ColorBuffer_Interpolate(120);
    Event_Wait(100);
    Audio_PlayCue(142);
    Event_Wait(30);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(60);
    Event_Wait(70);
    if (GameFlag_IsSet(0x982) == 0) {
        if (GameFlag_IsSet(0x983) == 0) {
            if ((gFrameCount & 1) != 0) {
                GameFlag_Set(0x982);
            } else {
                GameFlag_Set(0x983);
            }
        }
    }
    if (GameFlag_IsSet(0x982) == 0) {
        GameFlag_Set(0x982);
        GameFlag_Clear(0x983);
        Map_CopyCellsTo(103, 27, 89, 27, 7, 8);
        Map_CopyCellsTo(41, 90, 27, 92, 3, 2);
        Map_CopyCellsTo(41, 90, 29, 93, 3, 2);
        Map_CopyCellsTo(41, 90, 27, 94, 3, 2);
        Map_CopyCellsTo(41, 90, 27, 96, 3, 2);
        Map_CopyCellsTo(41, 90, 29, 97, 3, 2);
        Map_CopyCellsTo(41, 96, 25, 91, 3, 2);
        Map_CopyCellsTo(41, 92, 25, 93, 3, 2);
        Map_CopyCellsTo(41, 96, 25, 95, 3, 2);
        Map_CopyCellsTo(41, 96, 25, 97, 3, 2);
        Map_CopyCellsTo(41, 96, 27, 96, 3, 2);
        Map_CopyCellsTo(41, 96, 29, 97, 3, 2);
    } else {
        GameFlag_Set(0x983);
        GameFlag_Clear(0x982);
        Map_CopyCellsTo(111, 27, 89, 27, 7, 8);
        Map_CopyCellsTo(41, 90, 25, 91, 3, 2);
        Map_CopyCellsTo(41, 90, 25, 93, 3, 2);
        Map_CopyCellsTo(41, 90, 25, 95, 3, 2);
        Map_CopyCellsTo(41, 90, 25, 97, 3, 2);
        Map_CopyCellsTo(41, 90, 27, 96, 3, 2);
        Map_CopyCellsTo(41, 90, 29, 97, 3, 2);
        Map_CopyCellsTo(41, 94, 27, 92, 3, 2);
        Map_CopyCellsTo(41, 96, 29, 93, 3, 2);
        Map_CopyCellsTo(41, 94, 27, 94, 3, 2);
        Map_CopyCellsTo(41, 96, 27, 96, 3, 2);
        Map_CopyCellsTo(41, 96, 29, 97, 3, 2);
    }
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(20);
    Event_Wait(40);
    Camera_SetSpeed(0x8000, 0x1000);
    Camera_MoveTo(0x1c80000, -1, 0x21e0000, 1);
    Camera_WaitForMove();
    Event_Wait(50);
    Camera_MoveTo(0x1c80000, -1, 0x1a70000, 1);
    Camera_WaitForMove();
    Event_End();
    work->field_cb6 = 0;
}

void FieldScene_RunFlag986ActorOneScene(void)
{
    Struct_A *o;
    Struct_B *u;
    s32 g;
    s32 m1;
    s32 m2;
    s32 h;
    s32 k;

    g = 0x986;
    m1 = 0xcccc;
    m2 = 0x6666;
    h = 0x100;
    k = 0x338;
    Event_Begin();
    o = Actor_Get(12);
    if (o->unk8 >> 20 == 53) {
        if (GameFlag_IsSet(g) == 0) {
            GameFlag_Set(g);
            o = Actor_Get(ACTOR_PARTY_LEADER);
            if (o != 0) {
                Actor_SetPosition(ACTOR_GERALD, o->unk8, o->unk10);
            }
            Actor_SetSpeed(ACTOR_GERALD, m1, m2);
            Actor_WalkToAndWait(ACTOR_GERALD, k, 88);
            Actor_WalkToAndWait(ACTOR_GERALD, k, 104);
            Actor_FaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
            Event_Wait(20);
            Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
            Event_Wait(20);
            Event_SetMessage((s32)MsgVinasuHmmmWeCantPushBlock);
            Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
            Actor_FaceDirection(ACTOR_GERALD, 0, 10);
            Actor_ShowEmote(ACTOR_GERALD, h, 60);
            Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
            Event_Wait(20);
            Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
            Event_Wait(20);
            Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
            Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
            Event_Wait(30);
            Actor_WalkToAndWait(ACTOR_GERALD, k, 88);
            Actor_SetAnimation(ACTOR_GERALD, 2);
            u = Actor_Get(ACTOR_PARTY_LEADER);
            if (u != 0) {
                Actor_SetDestination(ACTOR_GERALD, u->unkA, u->unk12);
            }
            Actor_WaitForMove(ACTOR_GERALD);
            Actor_SetPosition(ACTOR_GERALD, 0, 0);
            Event_End();
        }
    }
}

void FieldScene_RunFiveCallSequence(void)
{

    Event_Begin();
    RunStagedActorTransition();
    Event_Wait(20);
    Event_End();
    FieldScene_RunFlag986ActorOneScene();
}

void SceneState_RunActor13AtColumn42Setup(void)
{
    Struct_1644 *obj;
    s32 val;
    s32 a;
    s32 b;

    obj = Actor_Get(13);
    Event_Begin();
    if (obj->unk8 >> 20 == 42) {
        Event_Wait(30);
        Audio_PlayCue(188);
        obj->unk55 = 0;
        val = 0xfffe0000;
        obj->unk14 = val;
        obj->unkC = val;
        GameFlag_Set(0x200);
        a = 3;
        b = 5;
        Map_CopyCellsTo(44, 117, 41, 117, a, b);
    }
    Event_End();
}

/* Floor switch under actor 0 (pixel x 676 to 683, z 788 to 795): stepping on it sets flag 0x201, redraws the switch cell and the 3x5 cells at (41, 117), plays cue 161 and lowers the actor two pixels; standing off it clears the flag and restores them. */
void VinasuHeya_UpdateFloorSwitch(void)
{
    struct FieldActor *leader = Object_GetById(0);
    s32 x = leader->x.part.pixel;
    s32 z = leader->z.part.pixel;

    if ((u32)(x - 676) > 7 || z < 788 || z >= 796) {
        Map_CopyCellsTo(53, 50, 42, 49, 1, 1);
        Map_CopyCellsTo(55, 117, 41, 117, 3, 5);
        GameFlag_Clear(0x201);
        leader->motion_flags |= 1;
        *(s32 *)leader->unknown_14 = 0;
        leader->y.fixed = 0;
    } else if (!GameFlag_IsSet(0x201)) {
        Engine_EventBegin();
        Engine_EventWait(5);
        Map_CopyCellsTo(52, 50, 42, 49, 1, 1);
        Map_CopyCellsTo(52, 117, 41, 117, 3, 5);
        GameFlag_Set(0x201);
        Engine_AudioPlayCue(161);
        leader->motion_flags &= ~1;
        *(s32 *)leader->unknown_14 = -0x20000;
        leader->y.fixed = -0x20000;
        Engine_EventEnd();
    }
}

void FieldScene_SetupActorTenCamera(void)
{
    u8 *state = *(u8 **)&gEventWork;
    {
        u16 *target = (u16 *)(state + 0xcba);
        s32 shown = 0;

        *target = shown;
    }
    {
        u16 *target = (u16 *)(state + 0xcb6);
        s32 shown = 1;

        *target = shown;
    }
    Event_Begin();
    Event_SetMessage((s32)MsgVinasuThoughtIdExploreAfterDoor);
    Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Event_ShowMessageAndWait(10, 0, 20);
    SetActorDirection(10, 57344, 0);
    Camera_SetSpeed(65536, 8192);
    Camera_MoveTo(29360128, -1, 28311552, 1);
    Camera_WaitForMove();
    Event_ShowMessage(10, 0);
    Event_End();
}

void Scene_RunActorLeapSequence(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 zero;

    zero = 0;
    rec7 = (s32)Object_GetById(0);
    Engine_EventBegin();
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    record = (s32)Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 0);
    {
        s32 shown = 0x4000;
    
        *(u16 *)(rec7 + 6) = shown;
    }
    Call3(Engine_ActorSetSpeed, 0, 0x30000, 0x18000);
    Engine_ActorMoveToAndWait(0, *(s16 *)(rec7 + 10), 0x228);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(0, 22);
    Engine_EventWait(30);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
    Engine_ActorRunRepeatedMotion(0, 2);
    Engine_EventWait(20);
    {
        s32 shown = 0xc000;
    
        *(u16 *)(rec7 + 6) = shown;
    }
    Engine_ActorSetAnimation(0, 5);
    Object_SetActionById(0, 24);
    Engine_EventWait(40);
    *(s32 *)(rec7 + 72) = 0x9999;
    {
        s32 z = *(s32 *)(rec7 + 16) + 0x480000;

        *(s32 *)(rec7 + 68) = zero;
        OverlayObject_SpawnWithMode14(*(s32 *)(rec7 + 8), 0, z, 223);
    }
    Engine_MapCopyCellAttributes(34, 35, 5, 1, 34, 34);
    OverlayObject_WaitUntilIdle(0);
    Engine_ActorSetChildValue(0, 15);
    Engine_EventRequestExit(20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
}

void FieldScene_PlaceAndPinSlots8To10(void)
{

    u32 i;
    Struct_18f8 *rec;
    s32 x;
    s32 y;
    s32 a;
    s32 b;

    Actor_Get(8);
    Event_Begin();
    x = 12;
    y = 44;
    Map_CopyCellAttributes(19, 44, 4, 1, x, y);
    x = 11;
    y = 51;
    Map_CopyCellAttributes(17, 51, 2, 2, x, y);
    i = 0;
    do {
        rec = Actor_Get(i + 8);
        a = rec->unk8 >> 20;
        b = rec->unk10 >> 20;
        Map_CopyCellAttributes(12, 50, 1, 1, a, b);
        i++;
    } while (i <= 2);
    SceneActor_SwapPositionsByDepth(10, 9);
    Event_End();
}

void SceneState_ApplyRectAt19_44AndRunThree(void)
{
    s32 x;
    s32 y;

    Event_Begin();
    x = 12;
    y = 44;
    Map_CopyCellAttributes(19, 44, 4, 1, x, y);
    RunStagedActorTransition();
    FieldScene_PlaceAndPinSlots8To10();
    Event_End();
}

void SceneActor_ApplyKind45AtActorsElevenAndTwelve(void)
{
    u32 i;
    Struct_199c *p;

    i = 0;
    do {
        p = Actor_Get(i + 11);
        i++;
        SetMapCellCollision(0, p->unk8, p->unk10, 45);
    } while (i <= 1);
}

void SceneActor_ApplyPositionsOfActors11And12(void)
{
    u32 i;
    Struct_19c0 *p;

    i = 0;
    do {
        p = Actor_Get(i + 11);
        if (p->unkC > -0x100000) {
            SetMapCellCollision(0, p->unk8, p->unk10, 255);
        }
        i++;
    } while (i <= 1);
}

void FieldScene_RunGuardedThreeStepSetup(void)
{

    Event_Begin();
    if (SceneActor_TryMoveActorZeroTwoTilesAhead() == 0) {
        SceneActor_ApplyKind45AtActorsElevenAndTwelve();
        RunStagedActorTransition();
        SceneActor_ApplyPositionsOfActors11And12();
    }
    Event_End();
}

void SceneState_MarkActorAndApplyRectAtTile(Struct_1a14 *obj)
{
    s32 x;
    s32 z;

    obj->unk23 |= 2;
    obj->unk55 = 0;
    x = obj->unk8 >> 20;
    z = obj->unk10 >> 20;
    Map_CopyCellAttributes(9, 24, 1, 1, x, z);
}

void OverlayObject_ResetObjectWhenFlatbs2Set(Struct_1a50 *o)
{
    Struct_Sub *q;
    s32 v;
    s32 z;
    s32 t;
    s32 m;

    q = o->unk50;
    v = q->unk9;
    if ((v & 12) == 12) {
        m = -13;
        m &= v;
        m |= 4;
        {
            u8 *pq = &q->unk9;
            *pq = m;
        }
        z = 0;
        o->unk44 = z;
        t = OverlayObject_SpawnWithMode14(o->unk8, 0, 0x2000000, 223);
        OverlayObject_WaitUntilIdle(o);
        o->unk8 = z;
        o->unk10 = z;
        Engine_ObjectDispatchRelease(t);
    } else {
        SceneActor_ApplyPositionsOfActors11And12();
    }
}

void SceneActor_UpdateSlots11And12ByTile(void)
{
    Struct_1a9c *o;

    Event_Begin();
    o = Actor_Get(11);
    if (o->unk8 >> 20 == 8) {
        ((s32 (*)())OverlayObject_WaitUntilIdle)();
        SceneState_MarkActorAndApplyRectAtTile(o);
    } else {
        OverlayObject_ResetObjectWhenFlatbs2Set(o);
    }
    o = Actor_Get(12);
    if (o->unk8 >> 20 == 7) {
        ((s32 (*)())OverlayObject_WaitUntilIdle)();
        SceneState_MarkActorAndApplyRectAtTile(o);
    } else {
        OverlayObject_ResetObjectWhenFlatbs2Set(o);
    }
    Event_End();
}
