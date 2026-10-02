/*
 * Canonical draft API context; no match or adoption is claimed.
 * API context measured 2026-10-02 with ordinary target flags:
 * all six prior successful objects retain every allocated byte and
 * normalized call/pool operand; no new match or adoption is claimed.
 * The shared void DispatchObject/u32 contract uses ordinary data casts;
 * every original matching-body and trial annotation is retained.
 */
#include "IWRAM_CALL.H"
#include "OBJDISP.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"

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
