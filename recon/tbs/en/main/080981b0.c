/* Draft, not exact (2026-09-26): 232 of 228 bytes, 66 differing halfwords.
   Complete rising-burst owner, including padding and its literal pool.
   Symbol constants recover the scale decrement and animation-id loads.
   Remaining: GCSE replaces the shared 0x10000 in the scale addition with
   a constant, rematerializing it instead of using sl; initial scheduling
   and final loop temporaries also differ. Signed scales and reading the
   stored base scale do not change that ancestry (allocator dump inspected). */
#include "FIXED_MATH.H"
#include "TYPES.H"
#include "OBJECT_EFX.H"

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

extern const u8 Value_fffff800;
extern const u8 Value_00000a3d;

void Audio_PlayCue(s32 sound);
void WaitFrames(s32 frames);
struct ParticleInstance *Object_Spawn(
    s32 kind, s32 x, s32 y, s32 z);
void Object_SetCallback(struct ParticleInstance *particle, const void *callback);
u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void Motion_SetTargetPositionFromMagnitudeAngle(
    struct ParticleInstance *particle, s32 magnitude, s32 angle);
void Object_Destroy(struct ParticleBurstEffect *effect);
void Func_080981b0(struct ParticleBurstEffect *effect)
{
    s32 count;
    s32 scale_step;
    s32 base_scale;

    Audio_PlayCue(0x9a);
    count = 30;
    scale_step = (s32)&Value_fffff800;
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
            s32 scale;
            s32 random;
            s32 speed;

            Object_SetCallback(particle, &Data_0809f0d4);
            scale = Random16();
            particle->base_scale = base_scale;
            scale += particle->base_scale;
            particle->scale = (s32)scale;
            particle->mode = 2;
            particle->animation_id = (s32)&Value_00000a3d;
            random = Random16();
            particle->random_offset = (s32)(random - Random16());
            speed = Random16() * 24 + 0x80000;
            Motion_SetTargetPositionFromMagnitudeAngle(
                particle, speed, Random16());
        }
        count--;
    } while (count >= 0);

    Audio_PlayCue(0x83);
    Object_Destroy(effect);
}
