#include "FIELD_EVENT.H"
#include "TYPES.H"
#include "GAME_STATE.H"
#include "FX_SCENE.H"
#include "OBJDISP.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"
#include "SOUND_IDS.H"
#include "GLOBAL_CELLS.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
#include "OBJECT_EFX.H"

void ObjectDispatch_SetSingleChildField26Far(void *, s32);
extern u8 Data_03001f30[];
void ObjectDispatch_ApplyValueToKind200Children(s32 battle_value);
s32 BattleFx_RunEventAction(void *resource, s32 battle_mode, s32 size);
void BattleEffect_SpawnBurstParticleField(void);

/* battle/effects/scene_transition/reset.c */

typedef struct {
    u8  reserved000[0xcb8];
    s16 active;
    s16 transition_timer;
} SceneTransitionState;

typedef struct {
    u8  reserved000[0x53c];
    u8 transition_status;
    u8 transition_mode;
    u8 transition_phase;
} SceneTransitionScene;

void *BattleFx_FindMatchingEvent(u32 kind, u32 entry_index, s32 *size);
void BattleFx_ApplyColorToTargetBuffer(u32 battle_value, s32 enabled);
void BattleFx_ApplyColorToSourceBuffer(u32 battle_value, s32 enabled);
void BattleFx_StartBufferInterpolation(s32 battle_value);
void Audio_PlayCue(s32 no);
void FieldEffect_WatchLeaderDistance(void);
extern struct BattleFxScene *gEffectWork;

/* battle/effects/burst_particles/run_main_object.c */
void Object_SetMode(void *, s32);
void BattleFx_PrepareBufferInterpolation(void);

/* battle/effects/burst_particles/run.c */
struct BurstParticleVector {
    s32 values[3];
};

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void Vector_AddPolarOffset(s32, s32, struct BurstParticleVector *);
void *Object_Spawn(s32, s32, s32, s32);
extern const u8 BattleFx_BurstParticleObjectScript[];

struct BurstParticleVisual {
    u8 reserved_00[5];
    u8 flags_a_low : 5;
    u8 flip : 1;
    u8 flags_a_high : 2;
    u8 reserved_06;
    u8 flags_b_low : 6;
    u8 flags_b_high : 2;
    union {
        struct { u16 shape : 10; u16 unused : 6; } bits;
        struct { u8 lo; u8 high_low : 4; u8 palette : 4; } bytes;
    } attributes;
};

struct BurstParticleVisualGroup {
    struct BurstParticleVisual primary;
    struct BurstParticleVisual child;
};


extern u8 BattleFx_CommonParticleScript[];
void BattleEffect_InitializeSharedScene(void);
void Animation_ApplyChildValuesFar(struct FieldActor *, s32);
u32 Random16(void);
void Object_SetPosition(struct FieldActor *, s32, s32, s32);
void Audio_PlayCue(s32);
void WaitFrames(s32);

struct EffectChild {
    u8 reserved_00[12];
    s32 y;
    u8 reserved_10[0x55 - 0x10];
    u8 flag;
    u8 reserved_56[0x6c - 0x56];
    void (*callback)(void);
};

void *BattleFx_SpawnItemBreakMode3(s32 x, s32 y, s32 z, s32 angle);
void Motion_SetTargetPositionFromMagnitudeAngle(
    void *object, s32 magnitude, s32 angle);
void Object_CommitPosition(void *object);
void Audio_PlayCue(s32 sound);
void ObjectGroup_ApplyRandomChildValues(void);
void UpdateRisingParticleBurst(void *effect);

void ResetSceneTransitionEffect(void)
{
    struct BattleFxScene **cell = &gEffectWork;
    struct BattleFxScene *ctx = *cell;
    SceneTransitionScene *scene = *(SceneTransitionScene **)((u8 *)cell - 0x64);
    SceneTransitionState *state = *(SceneTransitionState **)((u8 *)cell - 0x74);
    s16 zero;
    s32 size;
    void *resource;

    if (state->active != 0) {
        Audio_PlayCue(SOUND_SCENE_TRANSITION);
        Scheduler_RemoveCallback((u32)(FieldEffect_WatchLeaderDistance));

        zero = 0;
        state->active = zero;
        state->transition_timer = zero;
        ObjectDispatch_ApplyValueToKind200Children(0);

        BattleFx_ApplyColorToTargetBuffer(0x10000, 1);
        BattleFx_StartBufferInterpolation(1);
        BattleFx_ApplyColorToSourceBuffer(0, 0);
        BattleFx_ApplyColorToTargetBuffer(0x10000, 0);
        BattleFx_StartBufferInterpolation(30);
        WaitFrames(1);

        resource = BattleFx_FindMatchingEvent(0x40000005, 8, &size);
        if (resource != NULL)
            BattleFx_RunEventAction(resource, gGameState.selected_actor, size);

        if (ctx->child_option == 0) {
            scene->transition_phase = 0;
            scene->transition_status = 1;
            scene->transition_mode = 1;
            WaitFrames(10);
        }
    }
}

void BattleFx_RunBurstParticleMainObject(void)
{
    u8 *object;
    u8 *flags;
    u8 battle_value;

    object = FIELD_AT_OFFSET(*(void **)((u32)&Data_03001f30), u8 **, 0x14);
    if (object != 0) {
        BattleEffect_SpawnBurstParticleField();
        Object_SetMode(object, 2);
        object[0x59] = 0;
        ObjectDispatch_SetSingleChildField26Far(object, 0);
        flags = object + 0x23;
        battle_value = 2;
        battle_value |= *flags;
        *flags = battle_value;
        WaitFrames(0xAU);
        Audio_PlayCue(0x7E);
        WaitFrames(0x28U);
        BattleFx_PrepareBufferInterpolation();
    }
}

void BattleFx_RunBurstParticles(void)
{
    struct BattleFxScene *state = gEffectWork;
    struct BurstParticleVector position;
    struct BurstParticleVector *p;
    s32 entry_count;

    BattleEffect_SpawnBurstParticleField();
    Audio_PlayCue(SOUND_HEAVY_IMPACT);
    p = &position;
    entry_count = 4;
    do {
        void *object;
        s32 random_value;

        p->values[0] = state->x;
        p->values[2] = state->z;
        random_value = (Random16() * 6) + 0x40000;
        Vector_AddPolarOffset(random_value, Random16(), p);
        p->values[1] = state->y;
        object = Object_Spawn(
            0xD9,
            p->values[0],
            p->values[1],
            p->values[2]
        );
        if (object != 0) {
            ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)BattleFx_BurstParticleObjectScript);
            *((u8 *)object + 0x55) = 2;
        }
        WaitFrames((((u32)Random16() * 2) >> 16) + 2);
        entry_count--;
    } while (entry_count >= 0);
    WaitFrames(0x1E);
    BattleFx_PrepareBufferInterpolation();
}

/*
 * Spawns twenty-four burst particles around the effect target, two frames
 * apart, each drifting outwards from the scene centre.
 */
void BattleEffect_SpawnBurstParticleField(void)
{
    struct BattleFxScene *state;
    struct FieldActor *target;
    s32 position[3];
    s32 remaining;

    state = gEffectWork;
    target = state->main_object;
    BattleEffect_InitializeSharedScene();

    for (remaining = 0; remaining < 24; remaining++) {
        struct FieldActor *object;
        struct BurstParticleVisual *visual;
        struct BurstParticleVisual *child;
        s32 random_distance;

        if (state->angle == 0x4000) {
            position[0] = target->x.fixed;
            position[1] = target->y.fixed + 0xa0000;
            position[2] = target->z.fixed;
        } else if (state->angle == 0xc000) {
            position[0] = target->x.fixed;
            position[1] = target->y.fixed + 0x180000;
            position[2] = target->z.fixed;
        } else {
            position[0] = target->x.fixed;
            position[1] = target->y.fixed + 0xa0000;
            position[2] = target->z.fixed;
            Vector_AddPolarOffset(0xa0000, state->angle, position);
        }

        object = Object_Spawn(
            0x11c, position[0], position[1], position[2]);
        visual = &((struct BurstParticleVisualGroup *)object->sprite)->primary;
        child = &((struct BurstParticleVisualGroup *)object->sprite)->child;
        child->flip = visual->flip;
        child->flags_a_high = visual->flags_a_high;
        child->flags_b_high = visual->flags_b_high;
        child->attributes.bits.shape = visual->attributes.bits.shape;
        child->attributes.bytes.palette = visual->attributes.bytes.palette;

        if (object != 0) {
            object->scale_y = 0xb333;
            object->scale_x = 0xb333;
            object->acceleration = 0x18000;
            object->speed = 0x18000;
            object->motion_flags = 0;
            Animation_ApplyChildValuesFar(object, 11);
            Object_SetMode(object, 7);
            ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)(BattleFx_CommonParticleScript + 4));
            ObjectDispatch_SetSingleChildField26Far(object, 1);

            position[0] = state->x;
            position[1] = state->y;
            position[2] = state->z;
            if (state->angle == 0xc000)
                Vector_AddPolarOffset(0xe0000, state->angle, position);
            random_distance = Random16() * 6 + 0x40000;
            Vector_AddPolarOffset(random_distance, Random16(), position);
            Object_SetPosition(
                object, position[0], position[1], position[2]);
        }
        Audio_PlayCue(0x83);
        WaitFrames(2);
    }
    WaitFrames(8);
}

/* Links the scene child to the main object, spawns two mode-3 item-break
 * anchors on either side of the scene position, sends them outward, then
 * raises both anchors and the child together until the child has risen
 * 0x200000 before finishing both bursts. */
void BattleEffect_RunTargetedItemBreak(void)
{
    struct BattleFxScene *scene;
    void *main_object;
    void *child;
    void *anchors[2];
    s32 position[3];
    s32 x, y, z;
    s32 index;
    s32 start_y;

    scene = gEffectWork;
    child = scene->child;
    main_object = scene->main_object;
    if (child == 0)
        return;

    BattleEffect_InitializeSharedScene();
    *(void **)((u8 *)main_object + 0x68) = child;
    ObjectDispatch_InitializeFar((struct DispatchObject *)main_object, (u32)(BattleFx_CommonParticleScript + 12));

    x = scene->x;
    position[0] = x;
    y = scene->y + 0x100000;
    position[1] = y;
    z = scene->z;
    position[2] = z;
    anchors[0] = BattleFx_SpawnItemBreakMode3(x + 0x200000, y, z, 0x8000);
    anchors[1] = BattleFx_SpawnItemBreakMode3(
        position[0] - 0x200000, position[1], position[2], 0);

    WaitFrames(15);
    for (index = 0; index < 2; index++) {
        void *anchor = anchors[index];
        if (anchor != 0)
            Motion_SetTargetPositionFromMagnitudeAngle(
                anchor, 0xe0000, *(u16 *)((u8 *)anchor + 6));
    }

    Object_CommitPosition(anchors[0]);
    ((struct EffectChild *)child)->callback = ObjectGroup_ApplyRandomChildValues;
    Audio_PlayCue(130);
    ((struct EffectChild *)child)->flag = 4;

    start_y = ((struct EffectChild *)child)->y;
    if (anchors[0] != 0 && anchors[1] != 0 &&
        start_y <= start_y + 0x200000) {
        do {
            *(s32 *)((u8 *)anchors[0] + 0xc) += 0x4000;
            *(s32 *)((u8 *)anchors[1] + 0xc) += 0x4000;
            ((struct EffectChild *)child)->y += 0x4000;
            WaitFrames(1);
        } while (((struct EffectChild *)child)->y <= start_y + 0x200000);
    }

    UpdateRisingParticleBurst(anchors[0]);
    UpdateRisingParticleBurst(anchors[1]);
    BattleFx_PrepareBufferInterpolation();
}
