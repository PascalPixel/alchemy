#include "types.h"

s32 Func_0800439c(u32 value);
void Func_0800c62c(void);
void Func_0800c880(void);

void Func_0800c5fc(void) {
    Func_0800439c((u32)Func_0800c62c);
    Func_0800439c((u32)Func_0800c880);
    *(u16 *)0x04000000 &= 0xE1FF;
}
