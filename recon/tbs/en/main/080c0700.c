/* Draft, not exact (2026-09-24): 62 of 58 reference halfwords differ (128 vs 116 bytes).
   The literal pool lands after the level == 0 branch instead of at the end, and the IME write
   loads 0x0208 from the pool where the reference stores the low half of the IME address
   register itself (strh r3, [r3]). Plain stores instead of Dma_Set, inverted branches and
   u16/u32/volatile locals all keep the mid-function pool. */
#include "TYPES.H"
#include "DMA.H"

extern u8 *Data_03001e74;

void Graphics_ScaleRgb555Clamped(const u16 *source, volatile u16 *destination, s32 scale, s32 count);

/* Upload the battle palette's colours 0x60..0xdf to palette RAM, dimmed by
   level (0 copies them unchanged). Interrupts are held off meanwhile. */
void Unnamed_080c0700(s32 unused, s32 level)
{
    u8 *work;
    u16 *palette;
    s32 scale;
    volatile u32 interrupt_enable;

    work = Data_03001e74;
    palette = (u16 *)(work + 0x544);
    interrupt_enable = *(u16 *)0x04000208;
    *(u16 *)0x04000208 = 0x0208;
    if (level == 0) {
        Dma_Set(palette, (void *)0x050000c0, 0x80000080, (volatile u32 *)0x040000d4);
    } else {
        *(s32 *)(work + 0x644) = scale = 0x10000 - level * 1092;
        Graphics_ScaleRgb555Clamped(palette, (volatile u16 *)0x050000c0, scale, 0x80);
    }
    *(u16 *)0x04000208 = interrupt_enable;
}
