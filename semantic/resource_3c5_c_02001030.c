#include "types.h"

struct Obj {
    u8 pad00[6];
    u16 f06;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[36];
    s32 f38;
    u8 pad3c[4];
    s32 f40;
};

extern struct Obj *Func_02003e86();
extern void Func_02003e74();
extern void Func_02003f64();
extern void Func_02003eca();
extern void Func_02003f1c();
extern void Func_02003f1a();
extern void Func_02003f28();
extern void Func_02003fa4();
extern void Func_02003f3a();
extern void Func_02003f48();
extern void Func_02003fc4();
extern void Func_020038f8();

void Func_02001030(void) {
    struct Obj *p = Func_02003e86(0);
    s32 a;

    Func_02003e74();
    a = p->f08 >> 20;
    if (a == 6 || a == 18) {
        if ((p->f10 >> 20) == 20) {
            u16 h;

            p->f38 = 0x80000000;
            p->f40 = 0x80000000;
            Func_02003f64(0, 256, 20);
            Func_02003eca(0, 0x20000, 0x10000);
            Func_02003f1c(0, 4, 0);
            h = p->f06;
            if ((u16)(h + 0x4fff) <= 0x1fff || (u16)(h - 0x3001) <= 0x1fff) {
                Func_02003f1a(0, 16, 0);
                Func_02003f28(0);
                Func_02003fa4(0, 0x8000, 20);
            } else {
                Func_02003f3a(0, 0, -16);
                Func_02003f48(0);
                Func_02003fc4(0, 0x4000, 20);
            }
        }
    }
    Func_020038f8();
    Func_02003f1c();
}
