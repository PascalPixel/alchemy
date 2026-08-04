#include "layout_guard.h"
#include "types.h"

struct PaletteFade_080f2f10 {
    u8 padding0000[0x400];
    u16 accumulated[1536];
    u16 target[1536];
    u16 increments[1536];
    u16 palettes[2][512];
    u8 active_palette;
    s8 fade_frames;
    u8 fade_progress;
};

struct DmaTransfer_080f2f10 {
    const void *source;
    void *destination;
    u32 control;
};

struct TransferQueue_080f2f10 {
    volatile u16 count;
    u16 padding02;
    volatile struct DmaTransfer_080f2f10 entries[32];
};

LAYOUT_OFFSET_GUARD(
    PaletteFade080f2f10_Accumulated,
    struct PaletteFade_080f2f10,
    accumulated,
    0x400);
LAYOUT_OFFSET_GUARD(
    PaletteFade080f2f10_Target,
    struct PaletteFade_080f2f10,
    target,
    0x1000);
LAYOUT_OFFSET_GUARD(
    PaletteFade080f2f10_Increments,
    struct PaletteFade_080f2f10,
    increments,
    0x1c00);
LAYOUT_OFFSET_GUARD(
    PaletteFade080f2f10_Palettes,
    struct PaletteFade_080f2f10,
    palettes,
    0x2800);
LAYOUT_OFFSET_GUARD(
    PaletteFade080f2f10_Active,
    struct PaletteFade_080f2f10,
    active_palette,
    0x3000);

extern struct PaletteFade_080f2f10 *Data_03001ed0;

static void QueuePaletteTransfer_080f2f10(
    const void *source,
    void *destination)
{
    volatile u16 *interrupt_master = (volatile u16 *)0x04000208;
    volatile struct TransferQueue_080f2f10 *queue =
        (volatile struct TransferQueue_080f2f10 *)0x02002090;
    u16 saved_interrupt_master = *interrupt_master;
    u16 slot;

    /* The original disables interrupts with the low half of IME's address. */
    *interrupt_master = (u16)(u32)interrupt_master;
    slot = queue->count;
    if (slot <= 31) {
        volatile struct DmaTransfer_080f2f10 *entry =
            &queue->entries[slot];

        entry->source = source;
        queue->count = slot + 1;
        entry->destination = destination;
        entry->control = 0x84000080;
    }
    *interrupt_master = saved_interrupt_master;
}

/*
 * Advance the 512-color palette fade, render RGB555 colors into the inactive
 * double buffer, swap buffers, and queue the two 256-color palette transfers.
 */
void Func_080f2f10(void)
{
    struct PaletteFade_080f2f10 *fade = Data_03001ed0;
    u16 *rendered;
    s32 index;

    if (fade->fade_frames == 0)
        return;

    fade->fade_progress++;
    if ((s8)fade->fade_progress < fade->fade_frames) {
        for (index = 0; index < 1536; index++)
            fade->accumulated[index] += fade->increments[index];
    } else {
        volatile struct DmaTransfer_080f2f10 *dma =
            (volatile struct DmaTransfer_080f2f10 *)0x040000d4;

        dma->source = fade->target;
        dma->destination = fade->accumulated;
        dma->control = 0x84000300;
        fade->fade_frames = 0;
    }

    rendered = fade->palettes[fade->active_palette ^ 1];
    for (index = 0; index < 512; index++) {
        s32 component = index * 3;
        u16 red = fade->accumulated[component];
        u16 green = fade->accumulated[component + 1];
        u16 blue = fade->accumulated[component + 2];

        rendered[index] =
            (red & 0x7c00) |
            ((green >> 5) & 0x03e0) |
            ((blue >> 10) & 0x001f);
    }

    fade->active_palette ^= 1;
    rendered = fade->palettes[fade->active_palette];
    QueuePaletteTransfer_080f2f10(rendered, (void *)0x05000000);
    QueuePaletteTransfer_080f2f10(rendered + 256, (void *)0x05000200);
}
