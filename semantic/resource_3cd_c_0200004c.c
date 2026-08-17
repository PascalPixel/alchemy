#include "types.h"
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_020008f2(s32);
void Func_0200090e();

void Func_0200004c(void) {
    Func_020008f2(0x23CD);
    Func_0200090e(0xD, 0x102, 0);
    Func_0200090e(0xD, 0);
}
