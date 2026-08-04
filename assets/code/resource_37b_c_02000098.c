#include "types.h"
#define NULL ((void *)0)

void Func_020024b2(s32, s32, s32, s32, s32, s32);
void Func_02002618(s32);
void Func_020024ce(s32, s32, s32, s32, s32, s32);
void Func_0200251c(s32);
void Func_020024e6(s32, s32, s32, s32, s32, s32);
void Func_02002534(s32);
void Func_02002504(s32, s32, s32, s32, s32, s32);
void Func_02002514(s32, s32, s32, s32, s32, s32);
void Func_02002526(s32, s32, s32, s32, s32, s32);
void Func_02002542(s32, s32, s32, s32, s32, s32);
void Func_02002578(s32);
void Func_0200181c(void);

void Func_02000098(void) {
    s32 i;

    { s32 k5 = 2, k6 = 1; Func_020024b2(0, 28, 17, 8, k5, k6); }
    Func_02002618(200);
    for (i = 0; i != 22; i++) {
        Func_020024ce(10, 61, 17, 40, 2, 1);
        Func_0200251c(4);
        Func_020024e6(8, 61, 17, 40, 2, 1);
        Func_02002534(4);
    }
    { s32 k5 = 4, k6 = 3;
      Func_02002504(0, 59, 15, 38, k5, k6);
      Func_02002514(4, 59, 17, 38, k5, k6); }
    Func_02002526(8, 60, 17, 39, 2, 2);
    { s32 k5 = 17, k6 = 8; Func_02002542(0, 0, 2, 1, k5, k6); }
    Func_02002578(0x207);
    Func_0200181c();
}
