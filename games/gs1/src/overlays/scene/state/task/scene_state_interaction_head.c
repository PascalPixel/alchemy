#include "types.h"

#define SceneState_SetFlag331AndConfigureRegion46_17 Func_02000258
#define FieldScene_SetFlag332AndDrawTiles Func_0200028c
#define FieldScene_SetFlag333AndDrawTiles Func_020002c0
typedef struct Sub {
    u8 pad00[0x28];
    s16 *f28;
} Sub;

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[0x3c];
    Sub *f50;
    u8 f54;
} Obj;

extern u8 *Data_03001ebc;
extern u8 Value_0000008f;
extern u8 Value_00000090;
extern u8 Value_00002076;
extern u8 Value_00002078;
extern u8 Value_0000207a;
extern u8 Value_0000207c;

void Func_02004150(s32);
u8 *Func_020041b6(s32);
void Func_02004108(s32, s32, s32, s32, s32, s32);
void Func_02004184(s32);
u8 *Func_020041ea(s32);
void Func_0200413c(s32, s32, s32, s32, s32, s32);
void Func_020041b8(s32);
void Func_02004164(s32, s32, s32, s32, s32, s32);
s32 Func_02005a38(s32);
s32 Func_02005a42(s32);
void Func_02005ae8(s32, s32, s32);
s32 Func_02005a60(s32);
s32 Func_02005a6a(s32);
void Func_02005b0c(s32, s32, s32);
s32 Func_02005a84(s32);
s32 Func_02005a8e(s32);
void Func_02005b30(s32, s32, s32);
s32 Func_02005b86(s32);
Obj *Func_02005c38(s32);
void Func_02005c26(void);
Obj *Func_02005c46(s32);
void Func_02005d28(s32);
void Func_02005d40(s32, s32);
void Func_02005cb0();
void Func_02005cb0_a();
void Func_02005dec(void);
void Func_02005df8(void);
void Func_02005ca0(s32, s32);
void Func_02005de2(s32);
void Func_02005caa(s32);
void Func_0200396a(s32);
void Func_02005e26(void);
void Func_02005e3a(void);
void Func_02005d14(void);
void Func_02006084(void);
void Func_02005ebc(s32, s32);
void Func_02006002(s32);
void Func_0200601a(s32, s32);
s32 Func_02005f2c(s32);
s32 Func_02005f3c(s32);
s32 Func_02005f1e(s32);
void Func_02005f6c(s32);
void Func_02006052(s32);
s32 Func_02006062(s32, s32);
s32 Func_02005fda(s32, s32);

void SceneState_SetFlag331AndConfigureRegion46_17(void)
{
    u8 *p;

    Func_02004150(0x331);
    p = Func_020041b6(20) + 85;
    *p = 0;
    {
        s32 p5 = 44;
        s32 p6 = 17;

        Func_02004108(46, 17, 1, 1, p5, p6);
    }
}

void FieldScene_SetFlag332AndDrawTiles(void)
{
    u8 *slot;

    Func_02004184(0x332);
    slot = Func_020041ea(21) + 85;
    *slot = 0;
    {
        s32 v5 = 50;
        s32 v6 = 17;

        Func_0200413c(46, 17, 1, 1, v5, v6);
    }
}

void FieldScene_SetFlag333AndDrawTiles(void)
{
    Func_020041b8(0x333);
    {
        s32 width = 32;
        s32 height = 77;

        Func_02004164(32, 37, 1, 4, width, height);
    }
}

void Func_02001b30(void)
{
    {
        s32 x = Func_02005a38(896);
        s32 y = Func_02005a42(904);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Func_02005ae8(1, x, y);
    }
    {
        s32 x = Func_02005a60(912);
        s32 y = Func_02005a6a(920);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Func_02005b0c(2, x, y);
    }
    {
        s32 x = Func_02005a84(928);
        s32 y = Func_02005a8e(936);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Func_02005b30(3, x, y);
    }
}

void Func_02001c78(void)
{
    extern s32 Data_02000240[];

    u8 *state = Data_03001ebc;
    s32 v = Data_02000240[125];

    if (v != 0 && ((s32)(s16)*(u16 *)(state + 382) >> 10) == v
        && Func_02005b86(321) != 0) {
        u16 *p = (u16 *)(state + 386);
        s32 t = 99;

        *p = t;
    }
}

void Func_02001cc0(void)
{
    extern s32 Data_02000240[];

    u8 *state = Data_03001ebc;
    s32 best = 8;
    s32 bestd = 0x100000;
    s32 n = Data_02000240[125];
    Obj *p = Func_02005c38(n);
    s32 i;
    s32 *q;
    s32 base;

    Func_02005c26();
    for (i = 8; i <= 66; i++) {
        Obj *o = Func_02005c46(i);

        if (o != 0 && o->f54 == 1 && *o->f50->f28 == 165) {
            s32 dx = (p->f08 - o->f08) / 65536;
            s32 dy = (p->f10 - o->f10) / 65536;

            if (dy <= 0) {
                s32 a = dx;
                s32 d;

                if (a < 0) a = -a;
                if (dy < 0) dy = -dy;
                d = a + dy;
                if (d < bestd) {
                    best = i;
                    bestd = d;
                }
            }
        }
    }
    Func_02005d28(0x2085);
    Func_02005d40(best, 0);
    q = (s32 *)(state + 448);
    *q = 512;
    *(s32 *)(state + 456) = 15;
    Func_02005cb0(20);
    Func_02005dec();
    Func_02005df8();
    base = n << 4;
    Func_02005ca0(base + 880, p->f08 >> 20);
    {
        s32 v = p->f10 >> 20;

        Func_02005cb0_a(base + 888, v);
    }
    n++;
    if (n > 3) {
        Func_02005de2(10);
        Func_02005caa(282);
    } else {
        Func_0200396a(n);
        Func_02005e26();
        Func_02005e3a();
        *q = 0;
    }
    Func_02005d14();
}

s32 Func_02001ffc(s32 a, s32 b)
{
    extern s16 Data_02000240[];

    s32 v;
    s32 id;
    s32 r;

    Func_02006084();
    Func_02005ebc(b, 5);
    v = Data_02000240[224];
    if (v == (s32)&Value_0000008f) {
        id = (s32)&Value_00002076;
    } else if (v == (s32)&Value_00000090) {
        id = (s32)&Value_00002078;
    } else {
        id = (s32)&Value_0000207a;
    }
    Func_02006002(id);
    Func_0200601a(a, 0);
    if (Func_02005f2c(b + 512) != 0) {
        return 2;
    }
    if (Func_02005f3c(b + 520) != 0) {
        r = Func_02005f1e(0);
        if (r == 1) {
            return 2;
        }
        if (r == 2 || r == -1) {
            return 3;
        }
        return r;
    }
    Func_02005f6c(b + 520);
    Func_02006052((s32)&Value_0000207c);
    Func_02006062(a, 0);
    return Func_02005fda(0, 0);
}
