#include "types.h"

struct Slot {
    u16 f00;
    u16 f02;
    u16 f04;
    u16 f06;
};

extern struct Slot *Func_02003f12(s32);
extern void Func_02003fa4(void);
extern void Func_02003542(void);

void Func_020019cc(void)
{
    struct Slot *slot = Func_02003f12(0);

    if (slot->f06 == 0) {
        Func_02003fa4();
    } else {
        Func_02003542();
    }
}
