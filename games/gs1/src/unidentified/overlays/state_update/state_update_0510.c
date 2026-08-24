#include "types.h"

struct Slot {
    u16 f00;
    u16 f02;
    u16 f04;
    u16 f06;
};

extern struct Slot *Func_020042be(s32);
extern void Func_02004322(s32, s32, s32);
extern void Func_02003afc(void);

void Func_02001d78(void)
{
    struct Slot *slot = Func_020042be(0);

    if (slot->f06 == 0x4000) {
        Func_02004322(0, 6, 0);
    } else {
        Func_02003afc();
    }
}
