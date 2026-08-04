#include "types.h"

extern u8 *Data_03001ebc;

s32 Func_02007930(s32);
void Func_0200792c(s32, s32);
void Func_02007ada(s32);
void Func_02007960(s32);
void Func_0200791c(s32, s32, s32, s32, s32, s32);
void Func_0200797a(s32);
void Func_02007932(s32, s32, s32, s32, s32, s32);
void Func_02007990(s32);
void Func_020045d8(void);
void Func_0200798a(s32);

void Func_02002308(void)
{
    u8 *g = Data_03001ebc;

    if (*(s16 *)(g + 3256) == 0) {
        return;
    }
    if (Func_02007930(2375) != 0) {
        return;
    }
    Func_0200792c(5416, 1);
    Func_02007ada(188);
    Func_02007960(1);
    {
        s32 k5 = 17, k6 = 82;

        Func_0200791c(6, 77, 1, 2, k5, k6);
        Func_0200797a(5);
        Func_02007932(7, 77, 1, 2, k5, k6);
    }
    Func_02007990(1);
    Func_020045d8();
    Func_0200798a(2375);
}
