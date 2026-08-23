#include "fixed_math.h"
#include "types.h"

struct OrbitingParticleVector {
    s32 x;
    s32 y;
    s32 z;
};

struct OrbitingParticle {
    u8 padding_00[0x06];
    u16 rotation;
    struct OrbitingParticleVector position;
    u8 padding_14[0x04];
    s32 scale_x;
    s32 scale_y;
    u8 padding_20[0x18];
    struct OrbitingParticleVector orbit_center;
    u8 padding_44[0x20];
    u16 lifetime;
    u16 orbit_angle;
    u8 padding_68[0x04];
    void (*update)(struct OrbitingParticle *);
};

struct OrbitingParticleScene {
    u8 padding_00[0x04];
    struct OrbitingParticleVector origin;
    u8 padding_10[0x04];
    struct OrbitingParticle *main_particle;
    u8 padding_18[0x08];
    s8 skip_main_animation;
    u8 padding_21[0x13];
    s8 skip_main_finish;
};

struct OrbitingParticleGlobals {
    u8 padding_000[0x1f4];
    s32 resource_mode;
};

extern struct OrbitingParticleScene *Data_03001f30;
extern struct OrbitingParticleGlobals Data_02000240;

void Func_080030f8(s32 frames);
u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void Func_0800447c(
    s32 magnitude,
    s32 angle,
    struct OrbitingParticleVector *vector);
void Func_08009080(struct OrbitingParticle *particle, s32 mode);
void Func_08009240(struct OrbitingParticle *particle, s32 mode);
void *Func_0808e4b4(u32 kind, u32 index, s32 *size);
struct OrbitingParticle *Func_08096c80(s32 kind, s32 x, s32 y, s32 z);
s32 Func_08096b28(void *resource, s32 mode, s32 size);
void Func_08097384(void);
void Func_0809748c(void);
#define UpdateOrbitingParticleMain Func_08099018
#define UpdateOrbitingParticleLeft Func_08099070
#define UpdateOrbitingParticleRight Func_080990cc
void UpdateOrbitingParticleMain(struct OrbitingParticle *particle);
void UpdateOrbitingParticleLeft(struct OrbitingParticle *particle);
void UpdateOrbitingParticleRight(struct OrbitingParticle *particle);
void Func_080f9010(s32 sound_id);

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
    s32 count;

    scene = Data_03001f30;
    main_particle = scene->main_particle;
    Func_08097384();
    Func_080f9010(0x73);

    position_pointer = &position;
    count = 15;
    do {
        particle = Func_08096c80(0xe8, 0, 0, 0);
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
            Func_0800447c(magnitude, Rand(), position_pointer);
            particle->orbit_center.x = position_pointer->x;
            particle->orbit_center.y = position_pointer->y;
            particle->orbit_center.z = position_pointer->z;
        }

        Func_080030f8(3);
        count--;
    } while (count >= 0);

    Func_080030f8(10);
    Func_080f9010(0x73);
    Func_080030f8(50);

    if (main_particle != NULL && scene->skip_main_animation == 0) {
        Func_080f9010(0xd4);

        count = 15;
        do {
            Func_08009240(main_particle, 7);
            Func_080030f8(1);
            Func_08009240(main_particle, 0);
            Func_080030f8(4);
            count--;
        } while (count >= 0);

        if (scene->skip_main_finish == 0) {
            Func_080f9010(0xdc);
            Func_08009080(main_particle, 2);
        }

        main_particle->update = UpdateOrbitingParticleMain;
        resource = Func_0808e4b4(0x50000005, 6, &resource_size);
        if (resource != NULL) {
            Func_08096b28(
                resource,
                Data_02000240.resource_mode,
                resource_size);
        }
        Func_080030f8(20);
    }

    Func_0809748c();
}
