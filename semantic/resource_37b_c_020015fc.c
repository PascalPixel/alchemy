#include "types.h"
#define NULL ((void *)0)

void Func_02003a30(s32, s32, s32, s32);
s32 Func_020031fe(s32, s32, s32);
void Func_02003912(void);

void Func_020015fc(void) {
    Func_02003a30(2, 0x1600000, 0x700000, 0);
    if (Func_020031fe(12, 21, 7) != 0) {
        Func_02003912();
    }
}
