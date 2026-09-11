#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "object_efx.h"
#include "fixed_math.h"

/* battle/effects/orbiting_particles/update_main.c */
struct OrbitingParticle;

void BattleFx_UpdateOrbitingParticleMain(struct OrbitingParticle *particle)
{
    s32 battle_mode = *(s32 *)ADDR_03001E40 & 7;
    if (battle_mode == 0) {
        Battle_Apply(particle, 2);
    } else if (battle_mode == 2) {
        Battle_Apply(particle, 0);
    }
}

/* battle/effects/orbiting_particles/update_fade.c */
s32 Object_SetCallback(void *, s32);
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
      Object_SetCallback(object, gRom);
    }
  }
}

/* battle/effects/orbiting_particles/update_orbit_left.c */
struct OrbitingParticleVector {
    s32 x;
    s32 y;
    s32 z;
};

struct OrbitingParticle;

void RotateVectorByMagnitude(s32, s32, struct OrbitingParticleVector *);
void BattleFx_UpdateOrbitingParticleFade(void *object);

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
            RotateVectorByMagnitude(life << 17,
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
struct OrbitingParticleVector {
    s32 x;
    s32 y;
    s32 z;
};

struct OrbitingParticle;

void RotateVectorByMagnitude(s32, s32, struct OrbitingParticleVector *);
void BattleFx_UpdateOrbitingParticleFade(void *object);

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
            RotateVectorByMagnitude(battle_value << 17,
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

/* battle/effects/orbiting_particles/start.c */
struct OrbitingParticleChild {
    u8 reserved_00[35];
    u8 flags;
};

struct OrbitingParticleState {
    u8 reserved_00[20];
    struct OrbitingParticleChild *child;
    u8 reserved_18[8];
    u8 active;
    u8 reserved_21[20];
    s8 battle_mode;
};

extern struct OrbitingParticleState *gIw;
void BattleFx_RunOrbitingParticles(void);

void BattleFx_StartOrbitingParticles(void)
{
    struct OrbitingParticleState *state = gIw;
    struct OrbitingParticleChild *child = state->child;

    if (child != 0) {
        if (state->battle_mode != 0) {
            state->active = 1;
        }
        child->flags |= 2;
        BattleFx_RunOrbitingParticles();
    }
}

/* battle/effects/orbiting_particles/run.c */
struct OrbitingParticleVector {
    s32 x;
    s32 y;
    s32 z;
};

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

struct OrbitingParticleScene {
    u8 reserved_00[0x04];
    struct OrbitingParticleVector origin;
    u8 reserved_10[0x04];
    struct OrbitingParticle *main_particle;
    u8 reserved_18[0x08];
    s8 skip_main_animation;
    u8 reserved_21[0x13];
    s8 skip_main_finish;
};

struct OrbitingParticleGlobals {
    u8 reserved_000[0x1f4];
    s32 resource_mode;
};

extern struct OrbitingParticleScene *gIw;
extern struct OrbitingParticleGlobals gCell;

void WaitFrames(s32 frames);
u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void RotateVectorByMagnitude(
    s32 magnitude,
    s32 angle,
    struct OrbitingParticleVector *vector);
void Object_SetMode(struct OrbitingParticle *particle, s32 battle_mode);

void *Battle_Run(u32 kind, u32 entry_index, s32 *size);
struct OrbitingParticle *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);

void BattleFx_PrepareBufferInterpolation(void);
void BattleFx_UpdateOrbitingParticleMain(struct OrbitingParticle *particle);
void UpdateOrbitingParticleLeft(struct OrbitingParticle *particle);
void UpdateOrbitingParticleRight(struct OrbitingParticle *particle);
void Audio_PlayCue(s32 sound_id);

void BattleFx_RunOrbitingParticles(void)
{
    s32 resource_size;
    struct OrbitingParticleVector position;
    struct OrbitingParticleVector *p;
    struct OrbitingParticleScene *scene;
    struct OrbitingParticle *main_particle;
    struct OrbitingParticle *particle;
    void *resource;
    s32 entry_count;

    scene = gIw;
    main_particle = scene->main_particle;
    BattleFx_InitializeSharedScene();
    Audio_PlayCue(0x73);

    p = &position;
    entry_count = 15;
    do {
        particle = Object_Spawn(0xe8, 0, 0, 0);
        if (particle != NULL) {
            u32 initial_scale;
            s32 magnitude;

            initial_scale = (Rand() >> 1) + 0x8000;
            particle->scale_y = initial_scale;
            particle->scale_x = initial_scale;
            if ((Rand() & 1) != 0)
                particle->update = UpdateOrbitingParticleLeft;
            else
                particle->update = UpdateOrbitingParticleRight;

            particle->rotation = Rand();
            particle->lifetime = 60;
            particle->orbit_angle = Rand();
            Battle_Apply(particle, 9);

            p->x = scene->origin.x;
            p->y = scene->origin.y;
            p->z = scene->origin.z;
            magnitude = (Rand() << 2) + 0x20000;
            RotateVectorByMagnitude(magnitude, Rand(), p);
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

    if (main_particle != NULL && scene->skip_main_animation == 0) {
        Audio_PlayCue(0xd4);

        entry_count = 15;
        do {
            Battle_Apply(main_particle, 7);
            WaitFrames(1);
            Battle_Apply(main_particle, 0);
            WaitFrames(4);
            entry_count--;
        } while (entry_count >= 0);

        if (scene->skip_main_finish == 0) {
            Audio_PlayCue(0xdc);
            Object_SetMode(main_particle, 2);
        }

        main_particle->update = BattleFx_UpdateOrbitingParticleMain;
        resource = Battle_Run(0x50000005, 6, &resource_size);
        if (resource != NULL) {
            Battle_Place(
                resource,
                gCell.resource_mode,
                resource_size);
        }
        WaitFrames(20);
    }

    BattleFx_PrepareBufferInterpolation();
}

/* battle/effects/objects/update_shrinking_orbit.c */
struct Triple08099340 {
    s32 x;
    s32 y;
    s32 z;
};

extern s32 *gIw;
void RotateVectorByMagnitude(s32, s32, struct Triple08099340 *);
void Object_SetCallback(void *, void *);

void BattleFx_UpdateShrinkingOrbitObject(u8 *arg)
{
    s32 *global = gIw;
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
            RotateVectorByMagnitude(value << 16,
                          *(s16 *)(arg + 102) + (value << 11),
                          &local);
            *(s32 *)(arg + 8) = local.x;
            *(s32 *)(arg + 12) = local.y;
            *(s32 *)(arg + 16) = local.z;
        } else {
            Object_SetCallback(arg, gRom);
        }
    }
}

/* battle/effects/call/call_effect_03_and_stop.c */
void BattleFx_CallEffect03AndStop(void)
{
    Battle_Check();
    EffectRuntime_StopCurrentObject();
}
