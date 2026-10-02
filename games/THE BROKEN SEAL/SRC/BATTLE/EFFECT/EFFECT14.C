#include "TYPES.H"
#include "OBJDISP.H"
#include "FIELD_EFFECT.H"
#include "FIXED_MATH.H"
#include "IWRAM_CALL.H"
#include "SYSTEM.H"

struct RingOrigin {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
};

extern const u8 BattleFx_CommonParticleScript[];

struct OrbitEffect {
    u8 unknown_00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[20];
    s32 scale;
    u8 unknown_2c[28];
    s32 speed;
    u8 unknown_4c[24];
    u16 timer;
    s16 pause;
    struct OrbitEffect *anchor;
    void (*update)(struct OrbitEffect *);
};


void BattleFx_WanderAroundAnchor(struct OrbitEffect *effect);

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
s32 Object_SetPositionAndResetMotionFar(void *, s32, s32, s32);
s32 RunBattleEffect14();

void BattleFx_UpdateParticleMotionAndScale(union FieldObject *object)
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
    options.update = BattleFx_UpdateParticleMotionAndScale;
    options.start_scale_x = 0xCCCC;
    options.start_scale_y = 0xCCCC;
    i = 0;
    do {
        angle = i << 12;
        velocity[0] = Trig_Cos(angle) * 3 / 2;
        velocity[1] = 0;
        sine = Trig_Sin(angle);
        velocity[2] = sine;
        Effect_SpawnResident(origin->x, origin->y, origin->z, velocity[0], velocity[1], sine, 0x01090001, &options);
        i++;
    } while (i <= 16);
}

/* Drifts along a slowly turning heading with random pauses, then ends the
   effect after 101 frames. */
void BattleFx_WanderAroundAnchor(struct OrbitEffect *effect)
{
    s32 radius;
    s32 angle;
    s32 dx;
    s32 dz;

    radius = Random16() + 0x20000;
    angle = effect->angle;
    dx = Iwram_MulQ16(radius, Trig_Cos(angle));
    dz = Iwram_MulQ16(radius, Trig_Sin(angle));
    effect->x += dx;
    effect->z += dz;
    effect->angle += 0xfff0;
    if (effect->pause != 0) {
        effect->pause--;
        effect->angle += 0x800;
    } else if ((Random16() << 5) >> 16 == 0) {
        effect->pause = ((Random16() << 4) >> 16) + 8;
    }
    if (++effect->timer == 101)
        ObjectDispatch_InitializeFar((struct DispatchObject *)effect, (u32)BattleFx_CommonParticleScript);
}

/* Circles the anchor for 121 frames, then hands over to a random wander. */
void BattleFx_CircleAnchor(struct OrbitEffect *effect)
{
    struct OrbitEffect *anchor;
    s32 angle;
    s32 radius;
    s32 dx;
    s32 dz;

    anchor = effect->anchor;
    radius = 0x80000;
    angle = effect->angle;
    dx = Iwram_MulQ16(radius, Trig_Cos(angle));
    dz = Iwram_MulQ16(radius, Trig_Sin(angle));
    effect->x = anchor->x + dx;
    effect->z = anchor->z + dz;
    effect->angle += 0x800;
    if (++effect->timer == 121) {
        effect->update = BattleFx_WanderAroundAnchor;
        effect->timer = 0;
        effect->pause = 0;
        effect->speed = 0x1999;
        effect->scale = 0x30000;
        effect->angle = Random16();
    }
}

/* Battle effect 14: its object update and its entry. */
void BattleFx_ShrinkObjectScaleUntilHalf(void *obj)
{
    s32 scale;

    scale = FIELD_AT_OFFSET(obj, s32 *, 0x18) - 0x80;
    FIELD_AT_OFFSET(obj, s32 *, 0x1C) = scale;
    FIELD_AT_OFFSET(obj, s32 *, 0x18) = scale;
    if (scale < 0x8000) {
        Object_SetPositionAndResetMotionFar(obj, 0, 0, 0);
        FIELD_AT_OFFSET(obj, s32 *, 0x6C) = 0;
    }
}

void BattleFx_CallEffect14(void)
{
    RunBattleEffect14();
}
