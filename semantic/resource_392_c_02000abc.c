#include "types.h"
/* Protected-window palette adjustment owner, 0x02000abc-0x02000b23. */
  
 
extern void Func_0200164e();
extern u16 Func_02001614(u16, s32);
extern void Func_020016d2();
extern void Func_020016b6();
extern void Func_02001978();
void Func_02000abc(s32 adjustment)
{
    volatile u16 *palette = (volatile u16 *)0x05000000;
    u32 phase;
    Func_0200164e();
    for (phase = 0; phase <= 0x00df0000; phase += 0x10000) {
        u32 index = phase >> 16;
        u32 secondWindow = (index + 0xff3f) << 16;
        if ((u32)(phase + 0xffef0000) <= 0x60000 || secondWindow <= 0x70000)
            continue;
        palette[index] = Func_02001614(palette[index], adjustment);
    }
    Func_020016d2(); Func_020016b6(); Func_02001978(0x10000, 0);
}
