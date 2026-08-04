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

extern u8 Data_0200bfe4[];

extern u32 Func_02006b00(void);
extern s32 Func_02006b1c(void);
extern s32 Func_02006b22(void);
extern void Func_02006b3e(s32, s32, s32 *);
extern Obj *Func_02006bd2(s32, s32, s32, s32);
extern void Func_02006c3e(Obj *, s32);
extern void Func_02006be6(Obj *, s32);
extern void Func_02006bee(Obj *, s32);

void Func_02002fc4(Obj *a)
{
    s32 t[3];
    u32 n;

    if (a->f28 >= -255 && a->f28 <= 255) {
        a->f55 = 0;
    }
    n = Func_02006b00();
    if (n * 100 >> 16 <= 9) {
        Obj *o;
        s32 u;
        s32 w;

        t[0] = a->f08;
        t[1] = a->f0c;
        t[2] = a->f10;
        u = Func_02006b1c();
        w = Func_02006b22();
        Func_02006b3e(u << 4, w, t);
        {
            s32 x = t[0];
            s32 y = t[1];
            s32 z = t[2];

            o = Func_02006bd2(285, x, y, z);
        }
        if (o != 0) {
            o->f55 = 0;
            Func_02006c3e(o, 0);
            Func_02006be6(o, (s32)Data_0200bfe4);
            Func_02006be6(o, 1);
            Func_02006bee(o, 0);
        }
    }
}
