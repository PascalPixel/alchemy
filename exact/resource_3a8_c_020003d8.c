#include "types.h"

struct Obj {
    u8 filler00[6];
    u16 f06;
};

extern struct Obj *Func_02003ffa(s32);
extern s32 Func_02004168(s32, s32);
extern void Func_02003fee(void);
extern void Func_020040bc(s32);
extern s32 Func_020040e4(s32, s32);
extern void Func_02004008(void);

void Func_020003d8(void) {
    struct Obj *o;
    u32 v;

    o = Func_02003ffa(0);
    v = (o->f06 - 0x6001) << 16;
    if (v <= 0x7ffe0000) {
        Func_02004168(23, 23);
    } else {
        Func_02003fee();
        Func_020040bc(0x1ad1);
        Func_020040e4(23, 0);
        Func_02004008();
    }
}
