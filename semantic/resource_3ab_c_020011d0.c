#include "types.h"






extern void Func_02002baa(s32);
extern void Func_02002bb2(s32);
extern u8 * Func_02002be8(s32);
extern void Func_02002b3c(u8 *);
extern void Func_020023a6(void);
extern void Func_02002bf2(s32);
void Func_020011d0(void)
{
    u8 *progress = *(u8 **)0x03001ebc;
    u8 *leader;
    u32 x;
    s32 z;

    Func_02002baa(0x241);
    Func_02002bb2(0x240);
    leader = Func_02002be8(0);
    x = *(u32 *)(leader + 8);
    z = *(s32 *)(leader + 16);

    if (x >= 0x00680001 && x <= 0x00f00000 &&
        z > 0x00a00000 && z < 0x00f80000) {
        Func_02002b3c((u8 *)0x02009241);
        *(s16 *)(progress + 386) = 91;
    }

    Func_020023a6();
    Func_02002bf2(0x244);
}
