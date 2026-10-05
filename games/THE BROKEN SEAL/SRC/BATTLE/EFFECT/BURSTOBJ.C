#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "FIELDOBJ.H"
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
#include "MOTION_OBJECT.H"

#define Motion_SetTargetPositionFromMagnitudeAngle ((void (*)(void*,s32,s32))Motion_SetTargetPositionFromMagnitudeAngle)

#define BattleFx_ApplyColorToTargetBuffer ((void (*)(u32,s32))BattleFx_ApplyColorToTargetBuffer)
#define ObjectGroup_ApplyRandomChildValues ((void (*)(void))ObjectGroup_ApplyRandomChildValues)

void ObjectDispatch_SetSingleChildFlagsFar(void *, s32);

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
void BattleFx_ApplyColorToSourceBuffer(u32 battle_value, s32 enabled);
void Audio_PlayCue(s32 no);
extern struct BattleFxScene *gEffectWork;

void Object_SetMode(void *, s32);

struct BurstParticleVector {
    s32 values[3];
};

void Vector_AddPolarOffset(s32, s32, struct BurstParticleVector *);
void *Object_Spawn(s32, s32, s32, s32);

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

void Animation_ApplyChildValuesFar(struct FieldActor *, s32);
void Object_SetPosition(struct FieldActor *, s32, s32, s32);
void Audio_PlayCue(s32);

void Object_CommitPosition(void *object);
void Audio_PlayCue(s32 sound);

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
        Scheduler_RemoveCallback(FieldEffect_WatchLeaderDistance);

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
    struct FieldActor *object;

    object = gEffectWork->child;
    if (object != NULL) {
        BattleEffect_SpawnBurstParticleField();
        Object_SetMode(object, 2);
        object->collision_flags = 0;
        ObjectDispatch_SetSingleChildFlagsFar(object, 0);
        object->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
        WaitFrames(10);
        Audio_PlayCue(0x7e);
        WaitFrames(40);
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
        struct FieldActor *object;
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
            ObjectDispatch_InitializeFar((struct DispatchObject *)object, BattleFx_BurstParticleObjectScript);
            object->motion_flags = ACTOR_FALLS;
        }
        WaitFrames(((Random16() * 2) >> 16) + 2);
        entry_count--;
    } while (entry_count >= 0);
    WaitFrames(0x1E);
    BattleFx_PrepareBufferInterpolation();
}

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
            ObjectDispatch_InitializeFar((struct DispatchObject *)object, BattleFx_CommonParticleScript + 1);
            ObjectDispatch_SetSingleChildFlagsFar(object, 1);

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

void BattleEffect_RunTargetedItemBreak(void)
{
    struct BattleFxScene *scene;
    void *main_object;
    struct FieldActor *child;
    struct MotionObject *anchors[2];
    s32 position[3];
    s32 x, y, z;
    s32 index;
    s32 start_y;

    scene = gEffectWork;
    child = scene->child;
    main_object = scene->main_object;
    if (child == NULL)
        return;

    BattleEffect_InitializeSharedScene();
    *(void **)((u8 *)main_object + 0x68) = child;
    ObjectDispatch_InitializeFar((struct DispatchObject *)main_object, BattleFx_CommonParticleScript + 3);

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
        struct MotionObject *anchor = anchors[index];
        if (anchor != NULL)
            Motion_SetTargetPositionFromMagnitudeAngle(anchor, 0xe0000, anchor->angle);
    }

    Object_CommitPosition(anchors[0]);
    child->update = (void (*)(union FieldObject *))ObjectGroup_ApplyRandomChildValues;
    Audio_PlayCue(130);
    child->motion_flags = 4;

    start_y = child->y.fixed;
    if (anchors[0] != NULL && anchors[1] != NULL &&
        start_y <= start_y + 0x200000) {
        do {
            anchors[0]->y += 0x4000;
            anchors[1]->y += 0x4000;
            child->y.fixed += 0x4000;
            WaitFrames(1);
        } while (child->y.fixed <= start_y + 0x200000);
    }

    UpdateRisingParticleBurst(anchors[0]);
    UpdateRisingParticleBurst(anchors[1]);
    BattleFx_PrepareBufferInterpolation();
}
