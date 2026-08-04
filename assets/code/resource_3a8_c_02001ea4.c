#include "types.h"

struct Obj {
    u8 filler00[100];
    u16 f64;
    u16 f66;
};

extern u8 Data_0200beac[];

extern struct Obj *Func_02005ac6(s32);
extern u32 Func_020059e2(void);
extern void Func_02005a2e(struct Obj *, s32);

void Func_02001ea4(s32 a) {
    struct Obj *p;
    u32 t;

    p = Func_02005ac6(a);
    p->f64 = a;
    t = Func_020059e2();
    p->f66 = (t * 5) >> 12;
    Func_02005a2e(p, (s32)Data_0200beac);
}
