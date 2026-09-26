/* Draft, not exact (2026-09-25): 240 of 240 bytes, 64 differing halfwords.
   Split from WorldMap_LoadGraphics's listing and written from it; the
   DMA channel reset, the eight-word affine copy, the HBlank DMA and the
   line-range mode test all line up.
   Remaining: the ROM loads the work pointer after starting the DISPCNT
   read, keeps the page in r6 and DISPCNT in r5 (here r5 and r6), holds the
   DMA channel in r1 and derives the DMA destination from it (channel - 144)
   where this keeps the channel in r4 and reloads 0x04000020. */
#include "DMA.H"

struct MapAffineWork {
    u8 unknown_000[0x100];
    u16 next_start;
    u16 next_end;
    u16 start;
    u16 end;
    u16 pending;
};

/* Each frame: loads this frame's BG2/BG3 affine parameters from the
   double-buffered page and starts the HBlank DMA that streams the rest,
   latches the next line range, and switches the display into the affine
   mode only while that range is on screen. */
void MapAnimation_ApplyAffineFrame(void)
{
    u8 **pointers = (u8 **)0x03001e6c;
    u8 *pages = *pointers++ + 0xc80;
    s16 dispcnt = *(volatile u16 *)0x04000000 & 0xfff8;
    struct MapAffineWork *work = *(struct MapAffineWork **)pointers;
    volatile u16 *channel = (volatile u16 *)0x040000b0;
    u32 *src;
    u32 *dst;
    u32 mode;
    u32 start;

    channel[5] &= 0xc5ff;
    channel[5] &= 0x7fff;
    (void)channel[5];
    dst = (u32 *)0x04000020;
    if (pages != NULL) {
        src = (u32 *)(pages + (*(u32 *)0x03001e40 & 1) * 0x1400);
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst = *src++;
        Dma_Set(src, (void *)0x04000020, 0xa6600008, (volatile u32 *)channel);
    }
    work->start = work->next_start;
    work->end = work->next_end;
    start = work->start;
    mode = 0;
    if (start < 200) {
        mode = (work->end != 0) << 1;
        if (start <= work->end) {
            mode = 0;
            if (start == 0)
                mode = 2;
        }
    }
    *(volatile u16 *)0x04000000 = dispcnt | mode;
    work->pending = 0;
}
