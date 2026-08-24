#include "fixed_math.h"
#include "types.h"

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

extern struct OrbitingParticleScene *Data_03001f30;
extern struct OrbitingParticleGlobals Data_02000240;

void WaitFrames(s32 frames);
u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void RotateVectorByMagnitude(
    s32 magnitude,
    s32 angle,
    struct OrbitingParticleVector *vector);
void Object_SetMode(struct OrbitingParticle *particle, s32 battle_mode);
void Func_08009240(struct OrbitingParticle *particle, s32 battle_mode);
void *Func_0808e4b4(u32 kind, u32 entry_index, s32 *size);
struct OrbitingParticle *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
s32 Func_08096b28(void *resource, s32 battle_mode, s32 size);
void Func_08097384(void);
void BattleEffect_PrepareBufferInterpolation(void);
#define UpdateOrbitingParticleMain Func_08099018
#define UpdateOrbitingParticleLeft Func_08099070
#define UpdateOrbitingParticleRight Func_080990cc
void UpdateOrbitingParticleMain(struct OrbitingParticle *particle);
void UpdateOrbitingParticleLeft(struct OrbitingParticle *particle);
void UpdateOrbitingParticleRight(struct OrbitingParticle *particle);
void Audio_PlayCue(s32 sound_id);

#define RunOrbitingParticleEffect Func_08099160
void RunOrbitingParticleEffect(void)
{
    s32 resource_size;
    struct OrbitingParticleVector position;
    struct OrbitingParticleVector *position_pointer;
    struct OrbitingParticleScene *scene;
    struct OrbitingParticle *main_particle;
    struct OrbitingParticle *particle;
    void *resource;
    s32 entry_count;

    scene = Data_03001f30;
    main_particle = scene->main_particle;
    Func_08097384();
    Audio_PlayCue(0x73);

    position_pointer = &position;
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
            Func_08009240(particle, 9);

            position_pointer->x = scene->origin.x;
            position_pointer->y = scene->origin.y;
            position_pointer->z = scene->origin.z;
            magnitude = (Rand() << 2) + 0x20000;
            RotateVectorByMagnitude(magnitude, Rand(), position_pointer);
            particle->orbit_center.x = position_pointer->x;
            particle->orbit_center.y = position_pointer->y;
            particle->orbit_center.z = position_pointer->z;
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
            Func_08009240(main_particle, 7);
            WaitFrames(1);
            Func_08009240(main_particle, 0);
            WaitFrames(4);
            entry_count--;
        } while (entry_count >= 0);

        if (scene->skip_main_finish == 0) {
            Audio_PlayCue(0xdc);
            Object_SetMode(main_particle, 2);
        }

        main_particle->update = UpdateOrbitingParticleMain;
        resource = Func_0808e4b4(0x50000005, 6, &resource_size);
        if (resource != NULL) {
            Func_08096b28(
                resource,
                Data_02000240.resource_mode,
                resource_size);
        }
        WaitFrames(20);
    }

    BattleEffect_PrepareBufferInterpolation();
}
