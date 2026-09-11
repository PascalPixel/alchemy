#include "types.h"

#define GetOrbitingSceneObject Func_0200bb38
#define NormalizeOrbitingSceneObject Func_0200bad2
#define IsGameFlagSet Func_0200bb38_a
#define AllocateEffectTransfer Func_0200ba98
#define LoadEffectResource Func_0200bb60
#define ConfigurePaletteTransfer Func_0200bac8
#define ReleaseEffectTransfer Func_0200babe
#define UpdateOrbitingSceneObject Value_0200dae1
#define SceneEffect_UpdateOrbitPosition Func_02005ae0
#define InitializeStagedActorSceneOrbitingEffect Func_02005b48

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 palette;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;

extern u8 Value_0200dae1;

s32 Func_0200b99e(s32);
s32 Func_0200b9b4(s32);
s32 Func_0200b9ce(s32);
s32 Func_0200b9c4(void);
s32 Func_0200b9ca(void);
OrbitingSceneObject *Func_0200bb38();
s32 Func_0200bb38_a(s32);
void Func_0200bad2(OrbitingSceneObject *, s32);
u8 *Func_0200ba98(s32, s32);
void Func_0200bb60(s32);
s32 Func_0200bac8(u8, s32, u8 *);
void Func_0200babe(s32);

s32 SceneEffect_UpdateOrbitPosition(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Func_0200b99e(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Func_0200b9b4(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Func_0200b9ce(p[12] + 0x8000) / 8;
    a = Func_0200b9c4();
    b = Func_0200b9ca();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}

void InitializeStagedActorSceneOrbitingEffect(void)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = GetOrbitingSceneObject();
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    NormalizeOrbitingSceneObject(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (IsGameFlagSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = AllocateEffectTransfer(17, 0x608);
    LoadEffectResource(181);
    transfer += 0x400;
    ConfigurePaletteTransfer(sprite->palette, 128, transfer);
    ReleaseEffectTransfer(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)&UpdateOrbitingSceneObject;
    actor->state = zero;
}
