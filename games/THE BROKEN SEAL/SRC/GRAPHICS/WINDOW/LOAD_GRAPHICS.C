#include "TYPES.H"
#include "DMA.H"
#include "RESOURCE.H"

extern u8 Value_00000013;

/* Loads the shared window graphics: resource 0x13 into BG character block
   0, the window palette into background bank 15, the bank-15 frame colours
   and the object palettes. */
void Ui_LoadWindowGraphics(void)
{
    Dma_Set(GetResource((s32)&Value_00000013), (void *)0x06000000, 0x84000800,
            (volatile u32 *)0x040000d4);
    Dma_Set((const void *)0x0800777c, (void *)0x050001e0, 0x80000010,
            (volatile u32 *)0x040000d4);
    /* FAKEMATCH: the two do-while blocks keep the value-before-address
       order of the palette stores */
    do {
        s32 black = 0;
        *(volatile u16 *)0x05000000 = black;
    } while (0);
    do {
        volatile u16 *pal;
        s32 color;

        color = 0x4180;
        pal = (volatile u16 *)0x050001e8;
        *pal = color;
        color = 0x3960;
        pal++;
        *pal = color;
        color = 0x3140;
        pal++;
        *pal = color;
        color = 0x2920;
        pal++;
        *pal = color;
        color = 0x49a0;
        pal++;
        *pal = color;
        color = 0x51c0;
        pal++;
        *pal = color;
        color = 0x59e0;
        pal++;
        *pal = color;
    } while (0);
    Dma_Set((const void *)0x0800779c, (void *)0x05000200, 0x800000e0,
            (volatile u32 *)0x040000d4);
}
