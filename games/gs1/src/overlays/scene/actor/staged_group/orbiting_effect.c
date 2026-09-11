#include "types.h"

#define GetOrbitingSceneObject Func_02007664
#define NormalizeOrbitingSceneObject Func_0200762e
#define IsGameFlagSet Func_02007664_a
#define AllocateEffectTransfer Func_02007604
#define LoadEffectResource Func_02007694
#define ConfigurePaletteTransfer Func_0200762c
#define ReleaseEffectTransfer Func_0200762a
#define UpdateOrbitingSceneObject Value_0200b9d5
#define SceneEffect_UpdateLobeOrbitEffect26 Func_0200390c
#define SceneEffect_UpdateOrbitingEffect Func_020039d4
#define InitializeOrbitingRenderEffect Func_02003a3c

typedef struct RenderData {
    unsigned char pad00[0x1E];
    s16 rotation;
} RenderData;

typedef struct Effect {
    unsigned char pad00[8];
    s32 x;
    s32 y;
    unsigned char pad10[0x20];
    s32 angle;
    unsigned char pad34[4];
    s32 base_x;
    s32 base_y;
    unsigned char pad40[0x10];
    RenderData *render;
} Effect;

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

extern u8 Value_0200b9d5;

s32 Func_02007450(s32);
s32 Func_02007466(s32);
s32 Func_02007478(void);
s32 Func_0200747e(void);
s32 Func_02007480(s32);
Effect *Func_0200752e(s32);
s32 Func_02007512(s32);
s32 Func_02007528(s32);
s32 Func_02007540(void);
s32 Func_02007542(s32);
s32 Func_02007546(void);
OrbitingSceneObject *Func_02007664();
OrbitingSceneObject *Func_02007664_a();
void Func_0200762e(OrbitingSceneObject *, s32);
u8 *Func_02007604(s32, s32);
void Func_02007694(s32);
s32 Func_0200762c(u8, s32, u8 *);
void Func_0200762a(s32);

void SceneEffect_UpdateLobeOrbitEffect26(void)
{
    Effect *effect = Func_0200752e(26);
    RenderData *render = effect->render;
    s32 offset = Func_02007450(effect->angle) * 2;
    s32 first;

    if (offset > 0) {
        offset = -offset;
    }
    effect->x = effect->base_x + Func_02007466(effect->angle) * 2;
    effect->y = effect->base_y + offset;
    render->rotation = Func_02007480(effect->angle + 0x8000) >> 3;
    first = Func_02007478();
    effect->angle +=
        ((u32)(first << 9) >> 16)
        + ((u32)(Func_0200747e() << 9) >> 16)
        + 0x400;
}

s32 SceneEffect_UpdateOrbitingEffect(Effect *effect)
{
    RenderData *render = effect->render;
    s32 ofs = Func_02007512(effect->angle) * 2;
    s32 first;

    if (ofs > 0) {
        ofs = -ofs;
    }
    effect->x = effect->base_x + Func_02007528(effect->angle) * 2;
    effect->y = effect->base_y + ofs;
    render->rotation = Func_02007542(effect->angle + 0x8000) / 8;
    first = Func_02007540();
    effect->angle +=
        ((u32)(first << 9) >> 16)
        + ((u32)(Func_02007546() << 9) >> 16)
        + 0x400;
    return 0;
}

void InitializeOrbitingRenderEffect(void)
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
