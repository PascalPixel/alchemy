#include "types.h"







extern void Func_02002fa6(s32 strength);
extern u16 Func_02002f6c(u16 colour, s32 strength);
extern void Func_0200302a(void);
extern void Func_0200300e(void);
extern void Func_02003238(s32 value, s32 mode);
void Func_02001768(s32 strength)
{
    s32 phase;

    Func_02002fa6(strength);

    for (phase = 0; (u32)phase <= 0x00df0000; phase += 0x10000) {
        s32 index = (u32)phase >> 16;

        if ((u32)(phase + (s32)0xffef0000) > 0x60000 &&
            (u32)((index + 0xff3f) << 16) > 0x70000) {
            volatile u16 *palette = (volatile u16 *)0x05000000;
            palette[index] = Func_02002f6c(palette[index], strength);
        }
    }

    Func_0200302a();
    Func_0200300e();
    Func_02003238(0x10000, 0);
}
