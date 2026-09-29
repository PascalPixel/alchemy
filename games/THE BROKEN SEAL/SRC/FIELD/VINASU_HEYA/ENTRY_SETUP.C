#include "ENTRY_SETUP.H"
#include "IWRAM_CALL.H"
extern u8 *gActorEffectWork;

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
    *(s32 *)(*(u8 **)&gActorEffectWork + 24) = Actor_Get(ACTOR_PARTY_LEADER);
}

void SceneState_ClearWorkspaceWord24(void)
{
    *(s32 *)(*(u8 **)&gActorEffectWork + 24) = 0;
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
    do {
        velocity_x = effect->velocity[0];
        effect->position[0] += velocity_x;
        effect->position[1] += effect->velocity[1];
    } while (0);
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

    object = (struct SceneObject *)Engine_ActorGet(actor);
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
