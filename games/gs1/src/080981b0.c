#include "fixed_math.h"
#include "types.h"
#include "object_efx.h"

struct ParticleBurstEffect {
    u8 padding0[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 padding14[4];
    s32 scale_x;
    s32 scale_y;
};

struct ParticleInstance {
    u8 padding0[0x28];
    s32 random_offset;
    u8 padding2c[4];
    s32 scale;
    s32 base_scale;
    u8 padding38[0x10];
    s32 animation_id;
    u8 padding4c[9];
    u8 mode;
};

void Audio_PlayCue(s32 sound);
void WaitFrames(s32 frames);
struct ParticleInstance *Object_Spawn(
    s32 kind, s32 x, s32 y, s32 z);
void Object_SetCallback(struct ParticleInstance *particle, const void *callback);
u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void Object_SetVelocity(struct ParticleInstance *particle, s32 speed, s32 angle);
void Object_Destroy(struct ParticleBurstEffect *effect);
#define UpdateRisingParticleBurst Func_080981b0
void Func_080981b0(struct ParticleBurstEffect *effect)
{
    s32 count;
    s32 scale_step;
    s32 base_scale;

    Audio_PlayCue(0x9a);
    scale_step = -0x800;
    count = 30;
    do {
        effect->y += 0x10000;
        effect->angle += 0x2000;
        effect->scale_x += scale_step;
        effect->scale_y += scale_step;
        WaitFrames(1);
        count--;
    } while (count >= 0);

    count = 7;
    base_scale = 0x10000;
    do {
        struct ParticleInstance *particle;

        particle = Object_Spawn(
            0x11d, effect->x, effect->y, effect->z);
        if (particle != 0) {
            u32 scale;
            u32 random;
            s32 speed;

            Object_SetCallback(particle, &Data_0809f0d4);
            scale = Rand();
            particle->base_scale = base_scale;
            scale += base_scale;
            particle->scale = (s32)scale;
            particle->mode = 2;
            particle->animation_id = 0xa3d;
            random = Rand();
            particle->random_offset = (s32)(random - Rand());
            speed = Rand() * 24 + 0x80000;
            Object_SetVelocity(particle, speed, Rand());
        }
        count--;
    } while (count >= 0);

    Audio_PlayCue(0x83);
    Object_Destroy(effect);
}
