typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

struct Obj {
    s32 f00, f04, f08, f0c, f10, f14, f18, f1c;
    s32 f20, f24, f28, f2c, f30, f34, f38, f3c;
    s32 f40, f44, f48, f4c, f50, f54, f58, f5c;
    s32 f60;
    u16 f64;
};

void Func_02002ae2(struct Obj *);

s32 Func_020000cc(struct Obj *p) {
    p->f08 += p->f24;
    p->f10 += p->f2c;
    p->f2c += 0xf888f5c4;
    p->f18 += 0x600;
    p->f1c += 0x600;
    {
        s32 t = p->f64 - 1;
        p->f64 = t;
        if ((u16)t == 0) {
            Func_02002ae2(p);
        }
    }
    return 1;
}
#include "resource_38f.h"
