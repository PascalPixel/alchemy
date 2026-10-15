#include "iwram_call.h"
#include "objdisp.h"
#include "system.h"
#include "fixed_math.h"
#include "types.h"
#include "motobj.h"
#include "field_scene.h"
#include "field_effect.h"

extern const u8 gBattleFx_CommonParticleScript[];

struct OrbitEffect
{
    u8                  unk00[6];
    u16                 angle;
    s32                 x;
    s32                 y;
    s32                 z;
    u8                  unk14[20];
    s32                 scale;
    u8                  unk2c[28];
    s32                 speed;
    u8                  unk4c[24];
    u16                 timer;
    s16                 pause;
    struct OrbitEffect *anchor;
    void (*update)(struct OrbitEffect *);
};

void Effect_AdvanceMotion(struct FieldEffect *effect);
void Func_080df8e0(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags,
    const struct EffectOptions *options);

void BattleFx_SpawnRadialParticleRing(struct MotionObject *origin)
{
    struct EffectOptions options;
    s32                  velocity[3];
    s32                  sine;
    s32                  angle;
    u32                  i;

    options.palette = 0;
    options.update = Effect_AdvanceMotion;
    options.startScaleX = 0xCCCC;
    options.startScaleY = 0xCCCC;
    i = 0;
    do
    {
        angle = i << 12;
        velocity[0] = Trig_Cos(angle) * 3 / 2;
        velocity[1] = 0;
        sine = Trig_Sin(angle);
        velocity[2] = sine;
        /* FAKEMATCH: the native stores the sine before it reloads the origin (differ 2 without the barrier). */
        __asm__("");
        Func_080df8e0(
            origin->x, origin->y, origin->z, velocity[0], velocity[1], sine, 0x01090001, &options);
        i++;
    } while (i <= 16);
}

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
    /* FAKEMATCH: the empty one-pass block forms the pause address after the angle store (differ 2 without it). */
    do
    {
    } while (0);
    if (effect->pause != 0)
    {
        effect->pause--;
        effect->angle += 0x800;
    }
    else if ((Random16() << 5) >> 16 == 0)
    {
        effect->pause = ((Random16() << 4) >> 16) + 8;
    }
    if (++effect->timer == 101)
        ObjectDispatch_InitializeFar(
            (struct DispatchObject *)effect, (u32)gBattleFx_CommonParticleScript);
}
