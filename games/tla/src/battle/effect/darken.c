#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "ram_buffer.h"

struct BattleFxTransferWork
{
    u8 unknown_0000[0x7780];
    s32 transfer_mode;
    s32 transfer_value;
    u8 unknown_7788[0x78e4 - 0x7788];
    s32 frames_since_transfer;
    s32 transfer_pending;
};

void ColorBuffer_HalveNonzero(u8 *source, u8 *destination, s32 size);
void ColorBuffer_ScaleNonzeroThreeQuarters(u8 *source, u8 *destination, s32 size);

static __inline__ s32 BattlePres_FillWords(void *destination, s32 size, u32 value)
{
    return Iwram_FillWords(destination, size, value);
}

// Sends the 256x128 8bpp canvas to VRAM 0x06003500 on the frames its transfer
// is pending, then clears or fades it; a scheduler callback, as The Broken
// Seal's.
void BattlePres_ProcessPendingTileTransfer(void)
{
    struct BattleFxTransferWork *work;
    u8 *buf;

    /* FAKEMATCH: the one-pass block loads the effect work before the pending offset is started; assigned directly the offset is started first (differ 2). */
    do
    {
        work = (struct BattleFxTransferWork *)Ram_HeapSlots->battle_fx_work[0];
    } while (0);
    if (work->transfer_pending == 1)
    {
        buf = Ram_HeapSlots->battle_fx_work[1];
        switch (work->transfer_mode)
        {
        case 1: // Send it, then fill it with transfer_value.
            Dma_Set(buf, (void *)0x06003500, DMA_ENABLE32 | DMA_32BIT | 0x2000, REG_DMA3);
            /* FAKEMATCH: the fill goes through the inline wrapper BattlePres_FillWords, as ☀️'s IWRAM call layer; called directly, the size's split constant is finished before the buffer copy and the entry's pool load (differ 4). */
            BattlePres_FillWords(buf, 0x8000, work->transfer_value);
            break;
        case 2: // Afterimage: nonzero colour numbers drop to 1/2 or 3/4.
            if (work->transfer_value == 50)
                ColorBuffer_HalveNonzero(buf, (u8 *)0x06003500, 0x8000);
            else
                ColorBuffer_ScaleNonzeroThreeQuarters(buf, (u8 *)0x06003500, 0x8000);
            break;
        }
        work->transfer_pending = 0;
        work->frames_since_transfer = 1;
    }
    else
    {
        work->frames_since_transfer++;
    }
}

void Palette_DarkenSceneStep(void)
{
    u16 *color;
    s32 i;

    color = (u16 *)0x05000140;
    for (i = 0; i != 16; i++) {
        s32 blue = (*color >> 10) & 0x1f;
        s32 green = (*color >> 5) & 0x1f;
        s32 red = *color & 0x1f;

        blue--;
        green--;
        red--;

        if (blue < 0)
            blue = 0;
        if (green < 0)
            green = 0;
        if (red < 0)
            red = 0;
        *color++ = (blue << 10) | (green << 5) | red;
    }
    color = (u16 *)0x05000202;
    for (i = 0; i != 239; i++) {
        s32 blue = (*color >> 10) & 0x1f;
        s32 green = (*color >> 5) & 0x1f;
        s32 red = *color & 0x1f;

        blue--;
        green--;
        red--;

        if (blue < 0)
            blue = 0;
        if (green < 0)
            green = 0;
        if (red < 0)
            red = 0;
        *color++ = (blue << 10) | (green << 5) | red;
    }
}
