#include "IWRAM_CALL.H"
#include "SYSTEM.H"

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

s32 Trig_Cos(s32 angle);
s32 Trig_Sin(s32 angle);
void ObjectDispatch_InitializeFar(struct OrbitEffect *effect, s32 data);

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
        ObjectDispatch_InitializeFar(effect, 0x0809f0b0);
}
