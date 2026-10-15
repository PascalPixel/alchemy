/* Near miss, differ 2 of 69 (2026-10-15), ported from ☀️'s BattleFx_CircleAnchor
 * onto radial.c's OrbitEffect. One pair: the listing loads the angle before it starts
 * the 0x800 step (ldrh r3, [r7, #6]; movs r0, #128); this starts the step first. A
 * forced temporary with a one-pass block (as overlay 69e's s0810.c) gives differ 4, the
 * step held in an empty asm differ 12; no single barrier helped. Belongs after
 * radial.c's BattleFx_WanderAroundAnchor. */
#include "iwram_call.h"
#include "system.h"
#include "fixed_math.h"
#include "types.h"

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

void Func_080dfc3c(struct OrbitEffect *efx)
{
    struct OrbitEffect *anchor;
    s32 angle;
    s32 r;
    s32 dx;
    s32 dz;

    anchor = efx->anchor;
    r = 0x80000;
    angle = efx->angle;
    dx = Iwram_MulQ16(r, Trig_Cos(angle));
    dz = Iwram_MulQ16(r, Trig_Sin(angle));
    efx->x = anchor->x + dx;
    efx->z = anchor->z + dz;
    efx->angle += 0x800;
    if (++efx->timer == 121) {
        efx->update = BattleFx_WanderAroundAnchor;
        efx->timer = 0;
        efx->pause = 0;
        efx->speed = 0x1999;
        efx->scale = 0x30000;
        efx->angle = Random16();
    }
}
