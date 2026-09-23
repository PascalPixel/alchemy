/* Draft, not exact (2026-09-24): 70 differing halfwords, 240 of 240 bytes;
   same instructions. Residual: register allocation; the reference keeps
   the table in r6, the frame cells in ip and the affine cursor in r4, and
   derives the DMA destination as the channel less 144. */

#include "DMA.H"

extern u8 *Data_03001e6c[];
extern u32 Data_03001e40;

void Func_08010ff0(void)
{
    u8 **cells;
    u8 *table;
    u8 *frame;
    s32 display;
    u32 *source;
    u32 *affine;
    u32 top;
    u32 bottom;
    s32 bits;

    cells = Data_03001e6c;
    table = *cells++ + 0xc80;
    display = (s16)(*(u16 *)0x04000000 & 0xfff8);
    frame = *cells;
    {
        volatile u16 *channel = (volatile u16 *)0x040000b0;

        channel[5] &= 0xc5ff;
        channel[5] &= 0x7fff;
        (void)channel[5];
        affine = (u32 *)0x04000020;
        if (table != 0) {
            source = (u32 *)(table + (Data_03001e40 & 1) * 0x1400);
            *affine++ = *source++;
            *affine++ = *source++;
            *affine++ = *source++;
            *affine++ = *source++;
            *affine++ = *source++;
            *affine++ = *source++;
            *affine++ = *source++;
            *affine = *source++;
            Dma_Set(source, (void *)0x04000020, 0xa6600008, (volatile u32 *)channel);
        }
    }
    *(u16 *)(frame + 0x104) = *(u16 *)(frame + 0x100);
    *(u16 *)(frame + 0x106) = *(u16 *)(frame + 0x102);
    top = *(u16 *)(frame + 0x104);
    bits = 0;
    if (top <= 199) {
        bottom = *(u16 *)(frame + 0x106);
        bits = (bottom != 0) << 1;
        if (top <= bottom) {
            bits = 0;
            if (top == 0)
                bits = 2;
        }
    }
    *(u16 *)0x04000000 = display | bits;
    *(u16 *)(frame + 0x108) = 0;
}
