/* Draft, not exact (2026-09-24): 116 of 116 bytes, 38 halfwords differ (was
   62 at 128 bytes). Upload the battle palette colours 0x60..0xdf to palette
   RAM, dimmed by level (0 copies them unchanged), with interrupts held off.
   The IME idiom of SYSTEM/IO_WRITE_QUEUE.C (store the low half of the IME
   address itself) and a volatile saved word on the stack give the reference
   pool and frame. Residual: the reference computes work + 0x544 before the
   IME write and keeps it in r5 (pushing r5 and r6), where reading it at each
   use keeps it out of a register; with a palette local it lands in r0 and
   the function shrinks by 4 bytes. */
#include "TYPES.H"
#include "DMA.H"

extern u8 *Data_03001e74;
extern volatile u16 Data_04000208;
#define REG_IME Data_04000208

void Graphics_ScaleRgb555Clamped(const u16 *source, volatile u16 *destination, s32 scale, s32 count);

void Unnamed_080c0700(s32 unused, s32 level)
{
    u8 *work;
    u16 *palette;
    s32 scale;
    volatile u16 *ime;
    volatile u32 saved;

    work = Data_03001e74;
    ime = &REG_IME;
    saved = *ime;
    *ime = (u16)ime;
    if (level == 0) {
        Dma_Set(work + 0x544, (void *)0x050000c0, 0x80000080, (volatile u32 *)0x040000d4);
    } else {
        *(s32 *)(work + 0x644) = scale = 0x10000 - level * 1092;
        Graphics_ScaleRgb555Clamped((u16 *)(work + 0x544), (volatile u16 *)0x050000c0, scale, 0x80);
    }
    REG_IME = saved;
}
