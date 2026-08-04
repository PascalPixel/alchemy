#include "types.h"

struct Obj {
    u8 filler00[6];
    u16 f06;
};

extern struct Obj *Func_02003fb6(s32);
extern s32 Func_02004126(s32, s32);
extern void Func_02003fac(void);
extern void Func_0200407a(s32);
extern void Func_02004092(s32, s32);
extern void Func_02003fc6(void);

void Func_02000394(void) {
    struct Obj *o;
    u32 v;

    o = Func_02003fb6(0);
    v = (o->f06 - 0x2000) << 16;
    if (v > 0x80000000) {
        Func_02004126(22, 22);
    } else {
        Func_02003fac();
        Func_0200407a(0x1acf);
        Func_02004092(22, 0);
        Func_02003fc6();
    }
}
