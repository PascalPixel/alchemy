#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "ram_buffer.h"
#include "battle_work.h"

void ColorBuffer_Scale(u16 *source, u16 *destination, s32 scale, s32 count);

// パレットの明るさを設定する。level 0 は元の色をそのまま転送
void BattlePresentation_SetPaletteLevel(s32 unused, s32 level)
{
    struct BattleWork *screen = Ram_HeapSlots->battleWork;
    u16 *palette = screen->palette;
    u32 ime;
    u32 *slot;

    /* FAKEMATCH: the one-pass block and the loop on a zero local keep the IME save through slot's register, as in ☀️. ⚓️ also takes the slot's address as the block's first statement: taken at its declaration, the frame is opened before the palette offset is finished (one reordered pair, differ 2), and after the register's address the address is loaded first (differ 2). */
    do {
        u16 *ime_reg;
        s32 once;

        slot = &ime;
        ime_reg = (u16 *)0x04000208;
        once = 0;

        do {
            *slot = *ime_reg;
            *ime_reg = (u32)ime_reg;
        } while (once);
    } while (0);

    if (level == 0) {
        Dma_Set(palette, (void *)0x050000c0, 0x80000080, REG_DMA3);
    } else {
        ColorBuffer_Scale(
            palette, (u16 *)0x050000c0, screen->brightness = 0x10000 - level * 1092, 128);
    }
    {
        u32 saved = *slot;

        REG_IME = saved;
    }
}
