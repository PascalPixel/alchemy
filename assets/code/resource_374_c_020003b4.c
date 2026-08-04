#include "types.h"
#define NULL ((void *)0)

void Func_02002a54(void);
s32 Func_02002a32(s32);
void Func_02002b2c(s32);
s32 Func_02002b3c(s32, s32);
s32 Func_02002a94(s32, s32);
void Func_02002b58(s32, s32);
s32 Func_02002a74(s32);
void Func_02002b6e(s32);
void Func_02002b96(s32, s32);
void Func_02002b7e(s32);
void Func_02002ba6(s32, s32);
void Func_02002aca(void);

void Func_020003b4(void) {
    Func_02002a54();
    if (Func_02002a32(0x87a) != 0) {
        Func_02002b2c(0x1be8);
        Func_02002b3c(15, 0);
        if (Func_02002a94(0, 0) == 1) {
            Func_02002b58(15, 0);
        } else {
            u8 *p = *(u8 **)0x03001ebc;
            *(u16 *)(p + 472) = *(u16 *)(p + 472) + 1;
            Func_02002b96(15, 0);
        }
    } else if (Func_02002a74(0x815) != 0) {
        Func_02002b6e(0x1191);
        Func_02002b96(11, 0);
    } else {
        Func_02002b7e(0xea8);
        Func_02002ba6(11, 0);
    }
    Func_02002aca();
}
