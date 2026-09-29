/* 2026-09-29: with stock agscc the plain constants -0x800 and 0xa3d now
 * load from the pool like the reference, so the Value_ symbols are gone;
 * eight minutes of permutation also set the scale step before the start
 * cue. Score 515, from 660: 10 register-only, 1 reordered, 3 inserted, 1
 * deleted. What remains is the constant ownership the header describes: the
 * reference keeps the scale step in r5 and 0x10000 in sl across the loops,
 * where this rematerialises -0x800 inside the first loop and 0x10000 in the
 * scale addition. */
/* 2026-09-29: the fragment script is BattleFx_FragmentScript and the setter
 * Engine_ObjectSetScript, so the draft compiles again; alchemy permute
 * scores 660 (8 register-only, 1 operand for the Value_ constant, 4
 * inserted, 2 deleted). */
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


void Audio_PlayCue(s32 sound);
void WaitFrames(s32 frames);
struct ParticleInstance *Object_Spawn(
    s32 kind, s32 x, s32 y, s32 z);
void Engine_ObjectSetScript(struct ParticleInstance *particle, const void *callback);
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

    scale_step = -0x800;
    Audio_PlayCue(0x9a);
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
            s32 scale;
            s32 random;
            s32 speed;

            Engine_ObjectSetScript(particle, &BattleFx_FragmentScript);
            scale = Random16();
            particle->base_scale = base_scale;
            scale += particle->base_scale;
            particle->scale = (s32)scale;
            particle->mode = 2;
            particle->animation_id = 0xa3d;
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
