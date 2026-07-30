typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;

struct SceneObject {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[0x30];
    s32 speed;
    s32 range;
    u8 filler4c[9];
    u8 state;
};

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams {
    s32 unk00;
    s32 mode;
    s32 color1;
    s32 color2;
    u8 filler10[0x14];
    s32 callback;
};

extern struct SceneObject *Func_02005882(s32);
extern void Func_02005870(void);
extern void Func_02005972(s32, s32, s32, s32);
extern void Func_020057e6(void);
extern void Func_0200579c(s32);
extern void Func_020059d6(void);
extern void Func_020059ea(void);
extern void Func_020058a8(s32);
extern void Func_02005a1e(s32);
extern void Func_020058b8(s32);
extern s32 Func_02005812(s32);
extern s32 Func_02005816(s32);
extern void Func_02000b9a(s32, s32, s32, s32, s32, s32, s32, struct EffectParams *);
extern void Func_02005a8e(s32);
extern void Func_02005a0e(s32, s32);
extern void Func_0200599e(s32, s32);
extern void Func_02005906(s32, s32, s32);
extern void Func_02005914(s32, s32, s32);
extern void Func_02005920(void);
extern void Func_02005a42(s32, s32);
extern void Func_02005ac6(void);
extern void Func_0200597e(void);

void Func_020009c8(void)
{
    struct SceneObject *object;
    struct EffectParams params;
    struct Vec vec;
    u32 i;
    s32 v;
    s32 a;
    s32 b;
    s32 sx;
    s32 sy;
    s32 sz;
    s32 flash;
    s32 vol;

    a = -1;
    b = -1;
    sx = 0x50000;
    sy = 0x50000;
    sz = 0x10000;
    flash = 0x101;
    vol = 256;

    object = Func_02005882(0);
    Func_02005870();
    Func_02005972(-1, -1, -1, 0);
    Func_020057e6();
    Func_0200579c(1);
    object->y = 0x820000;
    object->range = 0x8000;
    object->speed = 0;
    object->state = 0;
    Func_020059d6();
    Func_020059ea();
    Func_020058a8(30);
    Func_02005a1e(204);
    object->state = 3;
    Func_020058b8(24);
    params.mode = 7;
    params.callback = 0x0200896d;
    params.color1 = 0xcccc;
    params.color2 = 0xcccc;
    for (i = 0; i <= 16; i++) {
        v = i << 12;
        vec.x = Func_02005812(v);
        vec.y = 0;
        vec.z = Func_02005816(v);
        vec.x = vec.x + vec.x / 2;
        Func_02000b9a(object->x, object->y, object->z, vec.x, vec.y, vec.z, 0x01090001, &params);
    }
    Func_02005a8e(188);
    Func_02005a0e(0, flash);
    Func_0200599e(0, 22);
    Func_02005906(sx, sy, sz);
    Func_02005914(a, b, 0xe666);
    Func_02005920();
    Func_02005a42(0, vol);
    Func_02005ac6();
    object->range = 0x10000;
    object->speed = 0x4000;
    Func_0200597e();
}
