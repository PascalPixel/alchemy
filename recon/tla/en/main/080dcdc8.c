/* Draft, differ 2 of 42: after the second mask the native stores the control
 * word and then starts the destination address (strh r2, [r3, #10];
 * movs r1, #128); this starts the address first. Measured: an empty asm
 * barrier before the volatile read gives that order but pulls the line
 * address's last shift (lsls r0, r0, #2) ahead of the store instead
 * (differ 2). Twin of The Broken Seal's BattleFx_ArmBg0HBlankDma. */
#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "ram_buffer.h"

struct WaveFxScroll {
    u16 lines[2][162];
    u8 unknown_288[2];
    u8 page;
    u8 unknown_28b[9];
    u8 delay;
};

// BG0 の走査線スクロールを HBlank DMA に掛ける
void Func_080dcdc8(void)
{
    struct WaveFxScroll *work = Ram_HeapSlots->unknown_4c[3];

    if (!work->delay) {
        u16 *line = work->lines[work->page];
        volatile u16 *dma = (volatile u16 *)0x040000b0;

        dma[5] &= 0xc5ff; // 開始タイミングとリピートを消す
        dma[5] &= 0x7fff; // 止める
        (void)dma[5];
        // HBlank ごとに 16 ビットを 1 回 (リピート、転送先リロード)
        Dma_Set(line, (void *)0x04000010, 0xa2600001, (volatile u32 *)dma);
    }
}
