#include "types.h"
#define NULL ((void *)0)

void Func_02003a08(s32, s32, s32, s32);
s32 Func_020031d6(s32, s32, s32);
void Func_0200383a(void);

void Func_020015d4(void) {
    Func_02003a08(2, 0xd00000, 0x700000, 0);
    if (Func_020031d6(10, 14, 7) != 0) {
        Func_0200383a();
    }
}
