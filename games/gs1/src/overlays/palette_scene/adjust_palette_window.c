#include "palette_scene.h"
/* Protected-window palette adjustment owner, 0x02001768-0x02000b23. */
/* Raw overlay relocation spellings for the five calls in this owner. */
void Func_02002fa6(); u16 Func_02002f6c(u16, s32); void Func_0200302a();
void Func_0200300e(); void Func_02003238();
void PaletteScene_AdjustPaletteWindow(s32 adjustment)
{
    volatile u16 *palette = (volatile u16 *)0x05000000;
    u32 phase;
    u32 next_phase;
    Func_02002fa6();
    phase = 0;
    do {
        u32 index = phase >> 16;
        u32 second_window;

        if ((u32)(phase + 0xffef0000) > 0x60000) {
            second_window = (index + 0xff3f) << 16;
            if (second_window > 0x70000)
                palette[index] = Func_02002f6c(palette[index], adjustment);
        }
        next_phase = phase + 0x10000;
        phase = next_phase;
    } while (next_phase <= 0x00df0000);
    Func_0200302a(); Func_0200300e(); Func_02003238(0x10000, 0);
}
