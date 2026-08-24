#include "types.h"

struct Obj {
    u8 filler00[6];
    u16 f06;
};

extern u8 *Data_03001ebc;

extern struct Obj *Func_02003f0a(s32);
extern void Func_02003ef6(void);
extern s32 Func_02003fb0(s32, s32, s32);
extern void Func_02003efe(s32);
extern void Func_02003fd4(s32);
extern s32 Func_02003fe4(s32, s32);
extern s32 Func_02003f3c(s32, s32);
extern void Func_02003fb8(s32, s32);
extern void Func_02004008(s32, s32);
extern void Func_02003fda(s32, s32);
extern void Func_0200402a(s32, s32);
extern void Func_02003f5e(void);

void Func_020002e8(void) {
    struct Obj *o;
    u32 v;
    u16 *q;

    o = Func_02003f0a(0);
    v = (o->f06 + 0xf9b4f001) << 16;
    if (v > 0x60000000) {
        Func_02003ef6();
        Func_02003fb0(0, 8, 0);
        Func_02003efe(10);
        Func_02003fd4(0x2584);
        Func_02003fe4(8, 0);
        if (Func_02003f3c(0, 0) == 0) {
            Func_02003fb8(8, 4);
            Func_02004008(8, 0);
        } else {
            q = (u16 *)(Data_03001ebc + 472);
            *q = *q + 1;
            Func_02003fda(8, 3);
            Func_0200402a(8, 0);
        }
        Func_02003f5e();
    }
}
