#include "types.h"
#include "iwram_call.h"

struct OrbitEffect
{
    u8  unk00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8  unk14[80];
    u16 timer;
    s16 pause;
};

u32  Random16Far(void);
s32  Math_Cosine(s32 angle);
s32  Math_Sine(s32 angle);
void Func_02003ff4(struct OrbitEffect *effect);

void Func_020009d0(struct OrbitEffect *effect)
{
    s32 radius;
    s32 angle;
    s32 dx;
    s32 dz;

    radius = Random16Far() + 0x20000;
    angle = effect->angle;
    dx = Iwram_MulQ16(radius, Math_Cosine(angle));
    dz = Iwram_MulQ16(radius, Math_Sine(angle));
    effect->x += dx;
    effect->z += dz;
    effect->angle += 0xfff0;
    /* FAKEMATCH: the empty one-pass block forms the pause address after the angle store, as in BattleFx_WanderAroundAnchor (differ 2 without it). */
    do
    {
    } while (0);
    if (effect->pause != 0)
    {
        effect->pause--;
        effect->angle += 0x800;
    }
    else if ((Random16Far() << 5) >> 16 == 0)
    {
        effect->pause = ((Random16Far() << 4) >> 16) + 8;
    }
    if (++effect->timer == 101)
        Func_02003ff4(effect);
}
