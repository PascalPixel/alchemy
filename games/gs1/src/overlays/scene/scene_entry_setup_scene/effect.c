#include "types.h"

#define SceneEffect_SpawnRandomEveryEightFramesB Func_020005e4
#define SceneEffect_SpawnTwoRandomizedParticles Func_020006a0
#define SceneEffect_RunObjectZeroColorSequence Func_02000754
#define SceneEffect_SpawnNineRadialEffects Func_02000b08
#define SceneEffect_SpawnRandomizedParticleEveryFourFrames Func_02000b98
#define SceneEffect_SpawnEffect284AtCell Func_02000c5c
struct Object_020005e4 { u8 unknown_00[8]; s32 x; s32 y; s32 z; };

struct EffectParams_020005e4 { s32 unk00; s32 mode; s32 color1; s32 color2; u8 unknown_10[0x18]; };

struct Object_020006a0 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams_020006a0 {
    s32 unk00;
    s32 mode;
    s32 color1;
    s32 color2;
    u8 unknown_10[0x18];
};

struct EffectObject {
    u8 filler00[0x30];
    s32 color;
    u8 filler34[0x38];
    s32 callback;
};

struct SceneObject {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams {
    s32 unk00;
    s32 mode;
    u8 filler08[0x1c];
    s32 callback;
};

struct SceneObject_02000b98 {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[0x24];
    s32 near;
    u8 filler3c[4];
    s32 far;
};

struct EffectParams_02000b98 {
    s32 unk00;
    s32 mode;
    s32 color1;
    s32 color2;
    s32 unk10;
    s32 unk14;
    u16 angle;
    u8 filler1a[0xa];
    s32 callback;
};

struct EffectObject_02000c5c {
    u8 filler00[0xc];
    s32 timer;
    u8 filler10[0x13];
    u8 mode;
    u8 filler24[0xc];
    s32 scale_x;
    s32 scale_y;
    u8 filler38[0x1d];
    u8 state;
    u8 filler56[3];
    u8 phase;
    u8 filler5a[9];
    u8 flag;
    u8 filler64[8];
    s32 callback;
};

extern u32 Data_03001e40;

s32 Func_020053b2();
s32 Func_020053d0();
s32 Func_020053e4();
s32 Func_020053f8();
s32 Func_020053f6();
void Func_0200078e();
s32 Func_02005470();
s32 Func_020054cc();
s32 Func_0200081a();
s32 Func_02000852();
struct EffectObject *Func_0200560a(s32);
void Func_020055f8(void);
void Func_02005766(s32);
void Func_02005676(s32, s32);
void Func_0200566a(s32, s32, s32);
void Func_02005678(s32);
void Func_020056c8(s32, s32);
s32 Func_02005646(s32);
void Func_020055e4(s32, s32);
void Func_02005636(s32);
void Func_0200577a(void);
void Func_02005786(void);
void Func_02005764(s16);
void Func_02005660(void);
struct SceneObject *Func_020059c2(void);
s32 Func_020058fc(s32);
s32 Func_02005900(s32);
s32 Func_020058ee(s32, s32);
void Func_02000c8a(s32, s32, s32, s32, s32, s32, s32, struct EffectParams *);
s32 Func_02005966(void);
void Func_02005bd0(s32);
s32 Func_020059a6(void);
s32 Func_020059a0(s32, s32);
s32 Func_020059be(void);
s32 Func_020059b8(s32, s32);
void Func_02000d52(s32, s32, s32, s32, s32, s32, s32, struct EffectParams *);
struct EffectObject *Func_02005a56(s32, s32, s32, s32);
void Func_02005ace(struct EffectObject *, s32);
void Func_02005a66(struct EffectObject *, s32);
void Func_02005a96(struct EffectObject *, s32);

s32 SceneEffect_SpawnRandomEveryEightFramesB(struct Object_020005e4 *object)
{
    struct EffectParams_020005e4 params;
    s32 phase, x, y, speed;
    phase = *(u32 *)0x03001e40 & 7;
    if (phase != 0) goto done;
    params.unk00 = 3 - (s32)((u32)(Func_020053b2() * 2) >> 16);
    params.color1 = 0x6666;
    params.color2 = 0x6666;
    params.mode = 14;
    x = object->x + (((s32)((u32)(Func_020053d0() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - (s32)((u32)(Func_020053e4() * 32) >> 16)) << 16);
    speed = Func_020053f6(((s32)((u32)(Func_020053f8() * 5) >> 16) << 16) + 0x00050000, 10);
    Func_0200078e(x, y, object->z, 0, speed, phase, 0x000b0000, &params);
done:
    return 0;
}

s32 SceneEffect_SpawnTwoRandomizedParticles(struct Object_020006a0 *obj)
{
    struct EffectParams_020006a0 params;
    s32 speed;
    s32 phase;

    params.color1 = 0x0000cccc;
    params.color2 = 0x0000cccc;
    params.unk00 = 0;

    speed = (s32)((u32)(Func_02005470() * 8) >> 16) * 0x3333;
    phase = Data_03001e40 & 15;

    Func_0200081a(
        obj->x + ((8 - phase) << 16),
        obj->y + 0x001a0000,
        obj->z,
        0,
        -speed,
        0,
        0x000a0000,
        &params);

    phase = Data_03001e40 & 15;
    if (phase == 0) {
        params.color1 = 0x00008000;
        params.color2 = 0x00008000;
        Func_02000852(
            obj->x
                + (((s32)((u32)(Func_020054cc() * 9) >> 16) - 4) << 16),
            obj->y,
            obj->z,
            0,
            0,
            0,
            0x000a0000,
            &params);
    }

    return 0;
}

void SceneEffect_RunObjectZeroColorSequence(void)
{
    struct EffectObject *obj;
    u8 *state;

    state = *(u8 **)0x03001ebc;
    obj = Func_0200560a(0);
    Func_020055f8();
    Func_02005766(228);
    obj->callback = 0x020086a1;
    obj->color = 0x3333;
    Func_02005676(0, 2);
    Func_0200566a(0, 0, -6);
    Func_02005678(0);
    Func_020056c8(0, 15);
    Func_020055e4(Func_02005646(0), 0);
    obj->callback = 0;
    Func_02005636(30);
    Func_0200577a();
    Func_02005786();
    Func_02005764(*(s16 *)(state + 0x16c));
    Func_02005660();
}

void SceneEffect_SpawnNineRadialEffects(void)
{
    struct SceneObject *object;
    struct Vec vec;
    struct EffectParams params;
    u32 i;
    s32 v;
    s32 x;
    s32 z;

    object = Func_020059c2();
    params.unk00 = 1;
    params.mode = 7;
    params.callback = 0x0200896d;
    for (i = 0; i <= 16; i += 2) {
        v = i << 12;
        vec.x = Func_020058fc(v);
        vec.y = 0;
        z = Func_02005900(v);
        x = vec.x;
        vec.z = z;
        x = x + Func_020058ee(x, 3);
        vec.x = x;
        Func_02000c8a(object->x, object->y, object->z, x, vec.y, z, 0x01030001, &params);
    }
}

s32 SceneEffect_SpawnRandomizedParticleEveryFourFrames(struct SceneObject_02000b98 *obj)
{
    struct EffectParams_02000b98 params;
    s32 y;
    s32 a;
    s32 b;
    s32 rnd;

    if ((Data_03001e40 & 3) != 0) {
        return 0;
    }
    if ((u32)(Func_02005966() * 6) >> 16 == 0) {
        if (obj->near != 0x80000000 || obj->far != 0x80000000) {
            Func_02005bd0(246);
        }
    }
    y = 0;
    params.angle = 286;
    params.color1 = 0x10000;
    params.color2 = 0x10000;
    params.unk10 = -327;
    params.unk14 = -327;
    rnd = Func_020059a6();
    a = Func_020059a0(((((u32)(rnd * 9)) >> 16) - 4) << 16, 10);
    rnd = Func_020059be();
    b = Func_020059b8(((((u32)(rnd * 9)) >> 16) - 4) << 16, 10);
    Func_02000d52(obj->x, obj->y, obj->z - 0x10000, a, y, b, 0x001c0001, &params);
    return 0;
}

struct EffectObject_02000c5c *SceneEffect_SpawnEffect284AtCell(s32 x, s32 z, s32 arg2)
{
    struct EffectObject_02000c5c *obj;
    s32 sx;
    s32 sz;

    sx = x << 16;
    sz = z << 16;
    obj = Func_02005a56(284, sx, 0, sz);
    if (obj == 0) {
        return 0;
    }
    obj->scale_x = 0x10000;
    obj->scale_y = 0x10000;
    Func_02005ace(obj, 0);
    Func_02005a66(obj, 7);
    obj->state = 0;
    obj->timer = 0;
    obj->phase = 0;
    obj->mode = 2;
    obj->callback = 0x02008b99;
    obj->flag = 0;
    Func_02005a96(obj, arg2);
    return obj;
}
