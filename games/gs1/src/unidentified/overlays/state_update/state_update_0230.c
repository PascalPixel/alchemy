#include "types.h"

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
};

extern struct Actor *Func_0200181a(s32);
extern void Func_02000dfc(void);
extern void Func_02000e32(void);

void Func_020006e4(void)
{
    struct Actor *actor = Func_0200181a(0);

    if (actor->f0c >= 0x100000) {
        Func_02000dfc();
    } else {
        Func_02000e32();
    }
}
