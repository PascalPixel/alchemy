#include "types.h"

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[20];
    s32 f28;
    u8 f2c[41];
    u8 f55;
} Obj;

extern u8 Data_0200ce50[];

extern u32 Func_02008568(void);
extern s32 Func_02008584(void);
extern s32 Func_0200858a(void);
extern void Func_020085a6(s32, s32, s32 *);
extern Obj *Func_0200863a(s32, s32, s32, s32);
extern void Func_020086b6(Obj *, s32);
extern void Func_0200864e(Obj *, s32);
extern void Func_02008656(Obj *, s32);

void Func_02003cf4(Obj *a)
{
    s32 t[3];
    u32 n;

    if (a->f28 >= -255 && a->f28 <= 255) {
        a->f55 = 0;
    }
    n = Func_02008568();
    if (n * 100 >> 16 <= 9) {
        Obj *o;
        s32 u;
        s32 w;

        t[0] = a->f08;
        t[1] = a->f0c;
        t[2] = a->f10;
        u = Func_02008584();
        w = Func_0200858a();
        Func_020085a6(u << 4, w, t);
        {
            s32 x = t[0];
            s32 y = t[1];
            s32 z = t[2];

            o = Func_0200863a(285, x, y, z);
        }
        if (o != 0) {
            o->f55 = 0;
            Func_020086b6(o, 0);
            Func_0200864e(o, (s32)Data_0200ce50);
            Func_0200864e(o, 1);
            Func_02008656(o, 0);
        }
    }
}
