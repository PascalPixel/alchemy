#include "RESOURCE.H"
#include "ANIMSPR.H"
#include "OBJECT_RUNTIME.H"
#include "SCRIPT_MOTION.H"
#include "FIELDOBJ.H"
/*
 * Battle effect 4: a ring of twelve screen-space particles, then a growing
 * burst object and three copies launched at the target, which share one
 * resource entry until their scripts finish.
 */
#include "TYPES.H"
#include "GAME_STATE.H"
#include "FX_SCENE.H"
#include "OBJDISP.H"
#include "EFFECT_SLOT.H"
#include "BATTLE_EFFECT_RUNTIME.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "OBJECT_EFX.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"

struct BurstPosition { s32 x, y, z; };



void WaitFrames(s32 frames);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct BurstPosition *pos);
void Object_SetMode(struct FieldActor *object, s32 mode);
extern const u8 BattleFx_BurstParticleObjectScript[];
extern struct BattleFxScene *gEffectWork;
void Object_SetPosition(struct FieldActor *object, s32 x, s32 y, s32 z);
s32 Object_CheckMovementCollision(struct FieldActor *object, struct BurstPosition *pos);
void Animation_ApplyChildValuesFar(struct FieldActor *object, s32 value);
void ObjectGroup_SetChildValueUnlessFifteenFar(s32 object, s32 value);
s32 ScriptObject_CheckOverlapFar(struct FieldActor *object, struct BurstPosition *pos);
s32 BattleFx_FindMatchingEvent(s32 flags, s32 group, s32 *context);
s32 BattleFx_RunEventAction(void *event, s32 object, s32 context);
struct AnimationObject *Object_ReplaceResourceEntry(struct AnimationObject *sprite, struct AnimationObject *resource);
struct FieldActor *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void BattleEffect_InitializeSharedScene(void);
void BattleFx_PrepareBufferInterpolation(void);
void Camera_WorldToScreen(struct BurstPosition *pos);
void EffectSlot_Initialize(struct EffectSlot *slot, s32 kind, s32 x, s32 z);
void BattleFx_UpdateRadialBurst(struct EffectSlot *slot);
void Audio_PlayCue(s32 cue);

static __inline__ void RaisedPosition(struct FieldActor *object, struct BurstPosition *pos)
{
    pos->x = object->x.fixed;
    pos->y = object->y.fixed + 0x100000;
    pos->z = object->z.fixed;
}

extern u8 Data_03001e40[];
s32 BattleFx_RunEventAction(void *resource, s32 battle_mode, s32 size);

/* battle/effects/orbiting_particles/update_main.c */
struct OrbitingParticle;

/* battle/effects/orbiting_particles/update_fade.c */

/* battle/effects/orbiting_particles/update_orbit_left.c */
struct OrbitingParticleVector {
    s32 x;
    s32 y;
    s32 z;
};

void BattleFx_UpdateOrbitingParticleFade(void *object);

/* battle/effects/orbiting_particles/start.c */
struct OrbitingParticleChild {
    u8 reserved_00[35];
    u8 flags;
};

void BattleFx_RunOrbitingParticles(void);

/* battle/effects/orbiting_particles/run.c */
struct OrbitingParticle {
    u8 reserved_00[0x06];
    u16 rotation;
    struct OrbitingParticleVector position;
    u8 reserved_14[0x04];
    s32 scale_x;
    s32 scale_y;
    u8 reserved_20[0x18];
    struct OrbitingParticleVector orbit_center;
    u8 reserved_44[0x20];
    u16 lifetime;
    u16 orbit_angle;
    u8 reserved_68[0x04];
    void (*update)(struct OrbitingParticle *);
};

void BattleFx_UpdateOrbitingParticleMain(struct OrbitingParticle *particle);
void BattleFx_UpdateOrbitingParticleLeft(struct OrbitingParticle *particle);
void BattleFx_UpdateOrbitingParticleRight(struct OrbitingParticle *particle);
void Audio_PlayCue(s32 sound_id);

struct Triple08099340 {
    s32 x;
    s32 y;
    s32 z;
};

struct SparkObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[24];
    s32 anchor_x;
    u8 unknown_3c[12];
    s32 speed;
    u8 unknown_4c[9];
    u8 mode;
    u8 unknown_56[8];
    u16 unknown_5e;
    u8 unknown_60[4];
    s16 phase;
    u8 unknown_66[6];
    void (*update)(struct SparkObject *);
};

struct SparkAnchor {
    u8 unknown_00[4];
    s32 x;
    s32 y;
    s32 z;
};

extern u32 gFrameCount;

void BattleFx_SwayParticle(u8 *object);

/* Twelve screen-space particles precede the main burst and its three
   copies. All copies share one resource entry until their scripts finish. */
void RunBattleEffect04(void)
{
    struct FieldActor *child;
    s32 event_context;
    struct FieldActor *spawned[4];
    struct BurstPosition pos;
    struct FieldActor *object;
    struct FieldActor *copy;
    struct FieldActor *target;
    struct BattleFxScene *scene;
    struct EffectSlot *slot;
    struct AnimationObject *resource;
    s32 event;
    s32 scale;
    s32 index;
    u8 resource_id;

    scene = gEffectWork;
    child = scene->child;
    BattleEffect_InitializeSharedScene();
    Audio_PlayCue(0x82);
    slot = scene->slots;

    for (index = 0; index <= 11; index++) {
        target = scene->main_object;
        RaisedPosition(target, &pos);
        Camera_WorldToScreen(&pos);
        EffectSlot_Initialize(slot, 0x11c, pos.x, pos.z);
        EffectSlot_SetCallback(slot, BattleFx_UpdateRadialBurst);
        EffectSlot_SetObjectMode(slot, 7);
        ObjectGroup_SetChildValueUnlessFifteenFar((s32)slot->object, 9);
        slot->scale_y = 0xb333;
        slot->scale_x = 0xb333;
        WaitFrames(2);
        slot++;
    }

    target = scene->main_object;
    pos.x = target->x.fixed;
    pos.y = target->y.fixed + 0x100000;
    pos.z = target->z.fixed;
    Vector_AddPolarOffset(0x80000, scene->angle, &pos);
    object = Object_Spawn(0xd7, pos.x, pos.y, pos.z);
    if (object == NULL) {
        BattleFx_PrepareBufferInterpolation();
        return;
    }
    object->scale_y = 0x4000;
    object->scale_x = 0x4000;
    object->facing = scene->angle;
    object->speed = 0x40000;
    object->acceleration = 0x40000;
    object->motion_flags = 0;
    Object_SetMode(object, 5);
    Animation_ApplyChildValuesFar(object, 3);
    scale = object->scale_x;
    if (scale < 0x10000) {
        do {
            scale += 0x500;
            object->scale_y = scale;
            object->scale_x = scale;
            WaitFrames(1);
            scale = object->scale_x;
        } while (scale <= 0xffff);
    }
    WaitFrames(3);
    resource = NULL;
    for (index = 2; index >= 0; index--) {
        copy = spawned[index] = Object_Spawn(0xd7, object->x.fixed, object->y.fixed, object->z.fixed);
        if (copy != NULL) {
            copy->scale_y = 0xf000;
            copy->scale_x = 0xf000;
            copy->facing = scene->angle;
            copy->speed = 0x40000;
            copy->acceleration = 0x40000;
            copy->motion_flags = 0;
            Object_SetMode(copy, 5);
            Animation_ApplyChildValuesFar(copy, 2);
            resource = Object_ReplaceResourceEntry((struct AnimationObject *)copy->sprite, resource);
        }
    }
    resource_id = resource->slot;
    if (scene->enabled != 0) {
        target = scene->main_object;
        pos.x = target->x.fixed;
        pos.y = target->y.fixed + 0x100000;
        pos.z = target->z.fixed;
        Vector_AddPolarOffset(0x380000, scene->angle, &pos);
    } else {
        pos.x = scene->x;
        pos.y = scene->y + 0x100000;
        pos.z = scene->z;
    }
    Object_SetPosition(object, pos.x, pos.y, pos.z);
    ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)(BattleFx_BurstParticleObjectScript + 0x10));
    for (index = 0; index < 3; index++) {
        copy = spawned[index];
        if (copy != NULL) {
            WaitFrames(3);
            Object_SetPosition(copy, pos.x, pos.y, pos.z);
            ObjectDispatch_InitializeFar((struct DispatchObject *)copy, (u32)(BattleFx_CommonParticleScript + 4));
        }
        }
    index = 0;
    if (((struct ScriptMotionObject *)object)->script != NULL) {
wait_script:
        WaitFrames(1);
        index++;
        if (index <= 59 && ((struct ScriptMotionObject *)object)->script != NULL)
            goto wait_script;
    }
    if (child != NULL && scene->child_mode == 0) {
        if (scene->child_option != 0)
            child->velocity_y = 0x80000;
        pos.x = child->x.fixed;
        pos.y = child->y.fixed;
        pos.z = child->z.fixed;
        Vector_AddPolarOffset(0x100000, scene->angle, &pos);
        if (Object_CheckMovementCollision(child, &pos) == 0 && ScriptObject_CheckOverlapFar(child, &pos) == 0) {
            child->acceleration = 0x10000;
            child->speed = 0x10000;
            Object_SetPosition(child, pos.x, pos.y, pos.z);
        }
    }
    event = BattleFx_FindMatchingEvent(0x50000005, 4, &event_context);
    if (event != 0)
        BattleFx_RunEventAction((void *)event, Data_02000240.object_id, event_context);
    WaitFrames(10);
    BattleFx_PrepareBufferInterpolation();
    WaitFrames(20);
    if (resource_id != 96)
        Resource_ResetEntry(resource_id);
}

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void BattleFx_UpdateOrbitingParticleMain(struct OrbitingParticle *particle)
{
    s32 battle_mode = *(s32 *)((u32)&Data_03001e40) & 7;
    if (battle_mode == 0) {
        Animation_ApplyChildValuesFar(particle, 2);
    } else if (battle_mode == 2) {
        Animation_ApplyChildValuesFar(particle, 0);
    }
}

void BattleFx_UpdateOrbitingParticleFade(void *object)
{
  s32 primary_fade;
  u8 *object_bytes;
  if (object != ((void *) 0))
  {
    primary_fade = *((s32 *)(((u8 *)object) + 0x18));
    primary_fade = primary_fade + 0xFFFFF000;
    *((s32 *)((object_bytes = (u8 *)object) + 0x1C)) = (s32)((*((s32 *)(object_bytes + 0x1C))) + 0xFFFFF000);
    *((s32 *)(object_bytes + 0x18)) = primary_fade;
    if (primary_fade <= 0x1000)
    {
      ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)BattleFx_CommonParticleScript);
    }
  }
}

void BattleFx_UpdateOrbitingParticleLeft(struct OrbitingParticle *particle)
{
    u8 *arg = (u8 *)particle;
    struct OrbitingParticleVector local;
    s16 life;
    s32 cnt;

    if (arg != 0) {
        cnt = *(u16 *)(arg + 100) - 1;
        *(u16 *)(arg + 100) = cnt;
        life = (s16)cnt;
        if (life != 0) {
            local.x = *(s32 *)(arg + 56);
            local.y = *(s32 *)(arg + 60);
            local.z = *(s32 *)(arg + 64);
            Vector_AddPolarOffset(life << 17,
                          *(s16 *)(arg + 102) + (life << 11),
                          &local);
            *(s32 *)(arg + 8) = local.x;
            *(s32 *)(arg + 12) = local.y;
            *(s32 *)(arg + 16) = local.z;
        } else {
            *(s32 *)(arg + 108) = (s32)BattleFx_UpdateOrbitingParticleFade;
        }
    }
}

/* battle/effects/orbiting_particles/update_orbit_right.c */
void BattleFx_UpdateOrbitingParticleRight(struct OrbitingParticle *particle)
{
    u8 *arg = (u8 *)particle;
    struct OrbitingParticleVector local;
    s16 battle_value;
    s32 raw_value;

    if (arg != 0) {
        raw_value = *(u16 *)(arg + 100) - 1;
        *(u16 *)(arg + 100) = raw_value;
        battle_value = (s16)raw_value;
        if (battle_value != 0) {
            local.x = *(s32 *)(arg + 56);
            local.y = *(s32 *)(arg + 60);
            local.z = *(s32 *)(arg + 64);
            Vector_AddPolarOffset(battle_value << 17,
                          *(s16 *)(arg + 102) - (battle_value << 11),
                          &local);
            *(s32 *)(arg + 8) = local.x;
            *(s32 *)(arg + 12) = local.y;
            *(s32 *)(arg + 16) = local.z;
        } else {
            *(s32 *)(arg + 108) = (s32)BattleFx_UpdateOrbitingParticleFade;
        }
    }
}

void BattleFx_StartOrbitingParticles(void)
{
    struct BattleFxScene *state = gEffectWork;
    struct OrbitingParticleChild *child = state->child;

    if (child != 0) {
        if (state->child_mode != 0) {
            state->enabled = 1;
        }
        child->flags |= 2;
        BattleFx_RunOrbitingParticles();
    }
}

void BattleFx_RunOrbitingParticles(void)
{
    s32 resource_size;
    struct OrbitingParticleVector position;
    struct OrbitingParticleVector *p;
    struct BattleFxScene *scene;
    struct OrbitingParticle *main_particle;
    struct OrbitingParticle *particle;
    void *resource;
    s32 entry_count;

    scene = gEffectWork;
    main_particle = scene->child;
    BattleEffect_InitializeSharedScene();
    Audio_PlayCue(0x73);

    p = &position;
    entry_count = 15;
    do {
        particle = Object_Spawn(0xe8, 0, 0, 0);
        if (particle != NULL) {
            u32 initial_scale;
            s32 magnitude;

            initial_scale = (Random16() >> 1) + 0x8000;
            particle->scale_y = initial_scale;
            particle->scale_x = initial_scale;
            if ((Random16() & 1) != 0)
                particle->update = BattleFx_UpdateOrbitingParticleLeft;
            else
                particle->update = BattleFx_UpdateOrbitingParticleRight;

            particle->rotation = Random16();
            particle->lifetime = 60;
            particle->orbit_angle = Random16();
            Animation_ApplyChildValuesFar(particle, 9);

            p->x = scene->x;
            p->y = scene->y;
            p->z = scene->z;
            magnitude = (Random16() << 2) + 0x20000;
            Vector_AddPolarOffset(magnitude, Random16(), p);
            particle->orbit_center.x = p->x;
            particle->orbit_center.y = p->y;
            particle->orbit_center.z = p->z;
        }

        WaitFrames(3);
        entry_count--;
    } while (entry_count >= 0);

    WaitFrames(10);
    Audio_PlayCue(0x73);
    WaitFrames(50);

    if (main_particle != NULL && scene->enabled == 0) {
        Audio_PlayCue(0xd4);

        entry_count = 15;
        do {
            Animation_ApplyChildValuesFar(main_particle, 7);
            WaitFrames(1);
            Animation_ApplyChildValuesFar(main_particle, 0);
            WaitFrames(4);
            entry_count--;
        } while (entry_count >= 0);

        if (scene->child_option == 0) {
            Audio_PlayCue(0xdc);
            Object_SetMode(main_particle, 2);
        }

        main_particle->update = BattleFx_UpdateOrbitingParticleMain;
        resource = BattleFx_FindMatchingEvent(0x50000005, 6, &resource_size);
        if (resource != NULL) {
            BattleFx_RunEventAction(
                resource,
                gGameState.selected_actor,
                resource_size);
        }
        WaitFrames(20);
    }

    BattleFx_PrepareBufferInterpolation();
}

/* Sways the object four units either side of its anchor x, stepping a
   128-step phase each frame. */
void BattleFx_SwayParticle(u8 *object)
{
    s16 *phase;

    phase = (s16 *)(object + 100);
    *(s32 *)(object + 8) = *(s32 *)(object + 56) + Iwram_MulQ16(0x40000, Trig_Sin(*phase << 9));
    (*phase)++;
    *phase = (*phase + 128) % 128;
}

void BattleFx_UpdateShrinkingOrbitObject(u8 *arg)
{
    s32 *global = gEffectWork;
    struct Triple08099340 local;
    s16 value;
    s32 raw;

    if (arg != 0) {
        raw = *(u16 *)(arg + 100) - 1;
        *(u16 *)(arg + 100) = raw;
        value = (s16)raw;
        if (value != 0) {
            local.x = global[1];
            local.y = global[2] + 0xA0000;
            local.z = global[3];
            Vector_AddPolarOffset(value << 16,
                          *(s16 *)(arg + 102) + (value << 11),
                          &local);
            *(s32 *)(arg + 8) = local.x;
            *(s32 *)(arg + 12) = local.y;
            *(s32 *)(arg + 16) = local.z;
        } else {
            ObjectDispatch_InitializeFar((struct DispatchObject *)arg, (u32)BattleFx_CommonParticleScript);
        }
    }
}

/* Sways the emitter above its anchor and, every third frame, spawns a
   swaying spark at a random offset around it. */
void BattleFx_RunSparkEmitter(struct SparkObject *object)
{
    struct SparkAnchor *anchor;
    s16 *phase;
    struct SparkObject *spark;
    s32 position[3];

    phase = &object->phase;
    anchor = gEffectWork;
    if (*phase != -1) {
        object->x = anchor->x + Iwram_MulQ16(0x60000, Trig_Sin(*phase << 10));
        object->y = anchor->y + 0x100000;
        object->z = anchor->z;
        (*phase)++;
        *phase = (*phase + 64) % 64;
    }
    if (gFrameCount % 3 == 0) {
        position[0] = object->x;
        position[1] = object->y + 0x20000;
        position[2] = object->z;
        Vector_AddPolarOffset(Random16() * 6, Random16(), position);
        spark = Object_Spawn(0x11d, position[0], position[1], position[2]);
        if (spark != 0) {
            spark->update = BattleFx_SwayParticle;
            spark->scale_x = spark->scale_y = 0x9999;
            spark->mode = 2;
            spark->speed = 458;
            spark->phase = Random16() >> 9;
            spark->anchor_x = spark->x;
            Animation_ApplyChildValuesFar(spark, 9);
            spark->unknown_5e = 72;
            ObjectDispatch_InitializeFar((struct DispatchObject *)spark, (u32)BattleFx_CommonParticleScript);
        }
    }
}
