#include "types.h"

struct SceneObject {
    u8 pad00[8];
    volatile s32 x;
    volatile s32 y;
    volatile s32 z;
};

struct EffectParams {
    u8 pad00[8];
    s32 color1;
    s32 color2;
    u8 pad10[18];
    u16 angle;
    u8 pad20[4];
};

extern volatile s32 Data_03001e40;
s32 Func_0200200e(void);
void Func_0200219e(s32);
void Func_020004d0(s32, s32, s32, s32, s32, s32, s32, struct EffectParams *);

#define SceneEffect_SpawnParticleEverySixteenFrames Func_02000344

s32 SceneEffect_SpawnParticleEverySixteenFrames(struct SceneObject *obj)
{
    volatile s32 *tick = &Data_03001e40;
    struct EffectParams params;
    s32 phase;

    if ((*tick & 7) == 0)
        Func_0200219e(118);

    phase = *tick & 15;
    if (phase != 0)
        return 0;

    params.color1 = 0xcccc;
    params.color2 = 0xcccc;
    params.angle = (u16)(((u32)(Func_0200200e() << 12) >> 16) + 0xf800);
    Func_020004d0(obj->x, obj->y, obj->z, 0, phase, phase, 0x880001, &params);
    return 0;
}
