typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;

struct Ent {
    s32 a;
    u16 b;
    u16 c;
};

extern u8 *Data_03001ebc;
extern struct Ent Data_0200d0e0[];

extern void Func_02004060(void);
extern s32 Func_02004092(s32);
extern void Func_02004214(s32);
extern s32 Func_0200403e(s32, s32, s32);
extern void Func_020040e4(s32, s32, s32);
extern s32 Func_020040da(s32);
extern void Func_02004148(s32, s32);
extern void Func_0200413c(s32, s32, s32);
extern void Func_020040ca(s32);
extern void Func_0200420a(s32);
extern void Func_0200421e(void);
extern void Func_0200422a(void);
extern void Func_020040ee(void);

void Func_02000464(void) {
    u8 *base;
    s16 *h;
    u32 i;
    s32 o;
    s32 k;
    u8 *p;

    base = Data_03001ebc;
    Func_02004060();
    for (i = 8; i <= 65; i++) {
        o = Func_02004092(i);
        if (o != 0) {
            *(u8 *)(o + 85) = 0;
        }
    }
    h = (s16 *)(base + 364);
    k = *h - 1;
    Func_02004214(158);
    { s32 t1 = Data_0200d0e0[k].b; s32 t2 = Data_0200d0e0[k].c; Func_0200403e(Data_0200d0e0[k].a, t1, t2); }
    Func_020040e4(0, 0x8000, 0x4000);
    p = (u8 *)(Func_020040da(0) + 85);
    *p = 0;
    Func_02004148(0, 2);
    Func_0200413c(0, 2, -8);
    Func_020040ca(10);
    Func_0200420a(*h);
    Func_0200421e();
    Func_0200422a();
    Func_020040ee();
}
