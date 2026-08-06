#include "types.h"







extern u8 * Func_02002cb0(s32);
extern u32 Func_02002bfa(void);
extern s32 Func_02002c1c(s32);
extern s32 Func_02002c28(s32);
extern void Func_02001446(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_020012a4(void)
{
    u8 *leader = Func_02002cb0(0);

    if ((*(u32 *)0x03001e40 & 15) == 0) {
        s32 angle = (s32)((((52 * Func_02002bfa()) >> 16) << 6) + 230);
        s32 x_velocity = Func_02002c1c(angle) / 4;
        s32 z_velocity = Func_02002c28(angle) / 2;

        Func_02001446(*(s32 *)(leader + 8), *(s32 *)(leader + 12),
                      *(s32 *)(leader + 16), x_velocity,
                      0, z_velocity, 0, 0);
    }
}
