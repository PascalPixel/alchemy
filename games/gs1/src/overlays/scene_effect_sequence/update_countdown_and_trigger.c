#include "types.h"

extern u8 Data_0200a69c[];
s32 Func_02003620();
void Func_02003662();
void Func_0200368a();
void Func_020037ca();

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void Func_02001ac8(void)
{
    s32 value;
    s32 *counter;

    counter = (s32 *)Data_0200a69c;
    if (*counter != 0) {
        if (--(*counter) != 40) {
            return;
        }
        Call3(Func_02003662, -1, -1, 0xe666);
    } else {
        value = Value0(Func_02003620);
        if (((u32)(((value << 4) - value) << 3) >> 16) == 0) {
            Func_020037ca(138);
            Call3(Func_0200368a, 0x10000, 0x20000, 0x10000);
            *counter = 80;
        }
    }
}
