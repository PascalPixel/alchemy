/* Battle effect 14: an object grows as it glides from the scene's main
   object to the scene position (offset in y) over eleven frames, then,
   unless the scene suppresses it, sixteen particles circle it while the
   secondary object shrinks away. */
#include "TYPES.H"
#include "FIXED_MATH.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"

struct EffectVector { s32 x, y, z; };

struct EffectParticle {
    u8 reserved_00[6];
    s16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 reserved_20[0x64 - 0x20];
    s16 timer;
    s16 phase;
    struct EffectParticle *parent;
    void (*callback)(void);
};

struct EffectScene {
    s32 variant;
    s32 x;
    s32 y;
    s32 z;
    struct EffectParticle *main_object;
    struct EffectParticle *secondary_object;
    u8 reserved_18[8];
    s8 flags;
};

extern struct EffectScene *gEffectWork;
void *Object_Spawn(s32, s32, s32, s32);
void BattleEffect_InitializeSharedScene(void);
void Object_SetMode(void *, s32);
void WaitFrames(s32);
void Audio_PlayCue(s32);
u32 Random16(void);
void Vector_AddPolarOffset(s32, s32, struct EffectVector *);
void Object_Destroy(void *);
void BattleFx_PrepareBufferInterpolation(void);
void BattleFx_ShrinkObjectScaleUntilHalf(void);
void BattleFx_CircleAnchor(void);

extern u8 Data_03001f30[];
s32 BattleEffect_RunFallbackObjectTransition();

/* battle/effects/radial_camera/update.c */
struct EffectPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectCamera {
    s32 filler_00;
    s32 x;
    s32 y;
    s32 z;
};

struct RadialCameraEffect {
    u8 filler_00[0xC];
    s32 x;
    s32 z;
    s32 source_x;
    s32 source_z;
    s32 filler_1c;
    s32 velocity;
    s32 acceleration;
    u8 filler_28[0xA];
    u16 field_32;
    u8 filler_34[0xC];
    s8 state;
    u8 filler_41;
    u8 flag;
};

extern void Camera_WorldToScreen(struct EffectPosition *);
extern s32 BattleFx_HasReachedTarget(struct RadialCameraEffect *);

void RunBattleEffect14(void)
{
    struct EffectScene *scene = gEffectWork;
    struct EffectParticle *main_object = scene->main_object;
    struct EffectParticle *secondary_object = scene->secondary_object;
    struct EffectVector particle_position;
    struct EffectVector origin;
    struct EffectVector target;
    struct EffectParticle *object;
    s32 step;
    struct EffectParticle *particle;
    s32 steps = 11;

    step = 0;
    origin.x = main_object->x;
    origin.y = main_object->y;
    origin.z = main_object->z;
    target.x = scene->x;
    target.y = scene->y - 0x40000;
    target.z = scene->z;
    object = Object_Spawn(0xda, 0, 0, 0);
    if (object == 0) {
        return;
    }
    BattleEffect_InitializeSharedScene();
    Object_SetMode(object, 2);
    do {
        s32 scale;
        object->x = origin.x + step * (target.x - origin.x) / 10;
        object->y = origin.y + step * (target.y - origin.y) / 10;
        object->z = origin.z + step * (target.z - origin.z) / 10;
        scale = 0x4000 + step * 0x10ccc / 10;
        object->scale_x = scale;
        object->scale_y = scale;
        WaitFrames(1);
        step++;
    } while (step < steps);
    object->scale_x = 0x1b333;
    object->scale_y = 0x14ccc;
    Audio_PlayCue(0xa3);
    WaitFrames(20);
    if (scene->flags == 0) {
        if (secondary_object != 0)
            secondary_object->callback = BattleFx_ShrinkObjectScaleUntilHalf;
        step = 0;
        do {
            s32 magnitude;

            particle_position.x = object->x;
            particle_position.y = object->y + step * 0xcccc + 0x40000;
            particle_position.z = object->z;
            magnitude = Random16() * 5 + 0x30000;
            Vector_AddPolarOffset(magnitude, Random16(), &particle_position);
            particle = Object_Spawn(0xf9, particle_position.x, particle_position.y, particle_position.z);
            if (particle != 0) {
                particle->callback = BattleFx_CircleAnchor;
                particle->parent = object;
                particle->timer = 0;
                particle->phase = 0;
                particle->angle = Random16();
            }
            WaitFrames(6);
            step++;
        } while (step <= 15);
        WaitFrames(20);
        WaitFrames(120);
    }
    Object_SetMode(object, 1);
    WaitFrames(30);
    Audio_PlayCue(0x88);
    WaitFrames(20);
    Object_Destroy(object);
    BattleFx_PrepareBufferInterpolation();
}

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void BattleFx_UpdateRadialCamera(struct RadialCameraEffect *effect)
{
    struct EffectCamera *camera;
    struct EffectPosition position;
    s8 *state_pointer;
    s16 angle;
    s32 state;

    camera = gEffectWork;
    state_pointer = &effect->state;
top:
    state = *state_pointer;
    if (state == 0) {
        position.x = effect->source_x;
        position.z = effect->source_z;
        angle = Random16();
        Vector_AddPolarOffset(Random16() * 30 + 0x280000, (u16)angle, &position);
        effect->x = position.x;
        effect->z = position.z;
        effect->acceleration = 0x40000;
        effect->velocity = 0x40000;
        effect->flag = state;
        goto advance;
    } else if (state == 1) {
        if (BattleFx_HasReachedTarget(effect)!= 0)
            return;
        *state_pointer = (u8)*state_pointer + 1;
        goto top;
    } else if (state == 2) {
        position.x = camera->x;
        position.y = camera->y + 0x80000;
        position.z = camera->z;
        Camera_WorldToScreen(&position);
        Vector_AddPolarOffset(0x40000, Random16(), &position);
        effect->x = position.x;
        effect->z = position.z;
        effect->field_32 = 0x1000;
        effect->flag = 1;
advance:
        *state_pointer = (u8)*state_pointer + 1;
        return;
    } else if (state == 3) {
        if (BattleFx_HasReachedTarget(effect) == 0)
            BattleFx_ClearOwnedSlot(effect);
        return;
    } else {
        return;
    }
}

/* battle/effects/misc/mark_child_and_run_fallback_transition.c */
void BattleFx_MarkChildAndRunFallbackTransition(void)
{
    FIELD_AT_OFFSET(FIELD_AT_OFFSET(*(void **)((u32)&Data_03001f30), void **, 0x14), s8 *, 0x5B) = 1;
    BattleEffect_RunFallbackObjectTransition();
}
