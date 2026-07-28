typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

struct SceneObject {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[0x24];
    s32 near;
    u8 filler3c[4];
    s32 far;
};

struct EffectParams {
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

extern u32 Data_03001e40;

extern s32 Func_02005966(void);
extern void Func_02005bd0(s32);
extern s32 Func_020059a6(void);
extern s32 Func_020059a0(s32, s32);
extern s32 Func_020059be(void);
extern s32 Func_020059b8(s32, s32);
extern void Func_02000d52(s32, s32, s32, s32, s32, s32, s32, struct EffectParams *);

s32 Func_02000b98(struct SceneObject *object)
{
    struct EffectParams params;
    s32 y;
    s32 a;
    s32 b;
    s32 r;

    if ((Data_03001e40 & 3) != 0) {
        return 0;
    }
    if ((u32)(Func_02005966() * 6) >> 16 == 0) {
        if (object->near != 0x80000000 || object->far != 0x80000000) {
            Func_02005bd0(246);
        }
    }
    y = 0;
    params.angle = 286;
    params.color1 = 0x10000;
    params.color2 = 0x10000;
    params.unk10 = -327;
    params.unk14 = -327;
    r = Func_020059a6();
    a = Func_020059a0(((((u32)(r * 9)) >> 16) - 4) << 16, 10);
    r = Func_020059be();
    b = Func_020059b8(((((u32)(r * 9)) >> 16) - 4) << 16, 10);
    Func_02000d52(object->x, object->y, object->z - 0x10000, a, y, b, 0x001c0001, &params);
    return 0;
}
