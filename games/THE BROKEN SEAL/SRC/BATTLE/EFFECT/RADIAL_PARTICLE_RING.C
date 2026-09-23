#include "TYPES.H"
#include "FIELD_EFFECT.H"

struct RingOrigin {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
};

s32 Func_0800231c(s32);
#define Engine_MathCos Func_0800231c
s32 Func_08002322(s32);
#define Engine_MathSin Func_08002322

void ParticleEffect_UpdateMotionAndScale(union FieldObject *object)
{
    struct FieldEffect *effect = &object->effect;

    effect->x += effect->velocity_x;
    effect->y += effect->velocity_y;
    effect->z += effect->velocity_z;
    effect->velocity_x -= effect->velocity_x / 18;
    effect->velocity_z -= effect->velocity_z / 16;
    effect->scale_x += effect->scale_rate_x;
    effect->scale_y += effect->scale_rate_y;
    effect->sprite->rotation += effect->spin;
}

void BattleFx_SpawnRadialParticleRing(struct RingOrigin *origin)
{
    struct EffectOptions options;
    s32 velocity[3];
    s32 sine;
    s32 angle;
    u32 i;

    options.palette = 0;
    options.update = ParticleEffect_UpdateMotionAndScale;
    options.start_scale_x = 0xCCCC;
    options.start_scale_y = 0xCCCC;
    i = 0;
    do {
        angle = i << 12;
        velocity[0] = Engine_MathCos(angle) * 3 / 2;
        velocity[1] = 0;
        sine = Engine_MathSin(angle);
        velocity[2] = sine;
        Effect_Spawn(origin->x, origin->y, origin->z, velocity[0], velocity[1], sine, 0x01090001, &options);
        i++;
    } while (i <= 16);
}
