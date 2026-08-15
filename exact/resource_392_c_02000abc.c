#include "types.h"
/* Protected-window palette adjustment owner, 0x02000abc-0x02000b23. */
/* Raw overlay relocation spellings for the five calls in this owner. */
void Func_0200164e(); u16 Func_02001614(u16, s32); void Func_020016d2();
void Func_020016b6(); void Func_02001978();
void Func_02000abc(s32 adjustment)
{
    volatile u16 *palette = (volatile u16 *)0x05000000;
    u32 phase;
    u32 nextPhase;
    Func_0200164e();
    phase = 0;
    do {
        u32 index = phase >> 16;
        u32 secondWindow;

        if ((u32)(phase + 0xffef0000) > 0x60000) {
            secondWindow = (index + 0xff3f) << 16;
            if (secondWindow > 0x70000)
                palette[index] = Func_02001614(palette[index], adjustment);
        }
        nextPhase = phase + 0x10000;
        phase = nextPhase;
    } while (nextPhase <= 0x00df0000);
    Func_020016d2(); Func_020016b6(); Func_02001978(0x10000, 0);
}
