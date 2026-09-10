#include "types.h"

#define GetOrbitingSceneObject Func_020059d4
#define NormalizeOrbitingSceneObject Func_020059a6
#define IsGameFlagSet Func_020059ec
#define AllocateEffectTransfer Func_0200599c
#define LoadEffectResource Func_02005a1c
#define ConfigurePaletteTransfer Func_020059c4
#define ReleaseEffectTransfer Func_020059c2
#define UpdateOrbitingSceneObject Value_0200aba5
#define InitializeOrbitingEffect Func_02002c0c

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

extern u8 Value_0200aba5;

s32 Func_020058aa(s32);
s32 Func_020058c0(s32);
s32 Func_020058da(s32);
s32 Func_020058d8(void);
s32 Func_020058de(void);
OrbitingSceneObject *Func_020059d4(void);
void Func_020059a6(OrbitingSceneObject *, s32);
s32 Func_020059ec(s32);
u8 *Func_0200599c(s32, s32);
void Func_02005a1c(s32);
s32 Func_020059c4(u8, s32, u8 *);
void Func_020059c2(s32);

s32 Func_02002ba4(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Func_020058aa(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Func_020058c0(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Func_020058da(p[12] + 0x8000) / 8;
    a = Func_020058d8();
    b = Func_020058de();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}

void InitializeOrbitingEffect(void)
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
