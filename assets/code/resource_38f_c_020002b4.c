#include "types.h"

u8 *Func_02002d10(s32);
void Func_02002d34(s32, s32, s32);
void Func_02002d94(s32, s32);
void Func_02002d88(s32, s32, s32);
void Func_02002e54(s32);

void Func_020002b4(s32 arg0) {
    u8 *p = Func_02002d10(0);
    p[0x55] = 0;
    Func_02002d34(0, 0x8000, 0x4000);
    Func_02002d94(0, 2);
    Func_02002d88(0, 0, -8);
    *(s32 *)(*(s32 *)0x03001ebc + 0x1c8) = 16;
    Func_02002e54(arg0);
}
#include "resource_38f.h"
