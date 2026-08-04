#include "types.h"

typedef void (*InterruptHandler)(void);

void Func_0800307c(s32 arg0, s32 arg1, InterruptHandler arg2);

void Func_08021e28(void) {
    u32 address = 0x04000012;
    s32 zero = 0;

    *(volatile u16 *)address = zero;
    address = 0x08021E15;
    Func_0800307c(2, 136, (InterruptHandler)address);
}
