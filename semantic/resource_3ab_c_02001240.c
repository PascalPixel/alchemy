#include "types.h"




extern u8 * Func_02002c4a(s32);
extern void Func_02002bb0(u8 *);
void Func_02001240(void)
{
    u8 *progress = *(u8 **)0x03001ebc;
    u8 *leader = Func_02002c4a(0);
    u32 x = *(u32 *)(leader + 8);
    s32 z = *(s32 *)(leader + 16);

    if (*(s16 *)(0x02000240 + 588) == 0 &&
        x >= 0x00900000 && x <= 0x00b00000 &&
        z >= 0x00a80000 && z < 0x00b00000) {
        Func_02002bb0((u8 *)0x02009241);
        *(s16 *)(progress + 386) = 91;
    }
}
