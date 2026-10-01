/* Draft, not exact: MapAnimation_ApplyAffineFrame, 240 bytes. 2026-10-01
   (wave 1, slice 1): 22 instructions differ, was 48. An int DISPCNT copy
   taken through (s16), the end line re-read into a u16 inside the range
   test and the DISPCNT store through an int parameter give the reference's
   code everywhere but the DMA0 channel pointer: the reference keeps it in
   r1, local to the stop sequence, so the copy pointer takes r4 and reload
   turns the DMA's two constants into adds r3, r1 and subs r1, #144; here
   CSE shares the pointer with the DMA call, it becomes global and takes
   r4. Assigning the stop sequence's last read to the pointer (channel =
   (void *)channel[5]) does make it local, but then it takes r2 and the
   other registers shift (31 differ). */
#include "DMA.H"

extern u8 *gMapAnimationPages[];
extern u32 gFrameCount;

struct MapAffineWork {
    u8 unknown_000[0x100];
    u16 next_start;
    u16 next_end;
    u16 start;
    u16 end;
    u16 pending;
};

static __inline__ void Io_Set16(s32 value, volatile u16 *reg)
{
    *reg = value;
}

/* Each frame: loads this frame's BG2/BG3 affine parameters from the
   double-buffered page and starts the HBlank DMA that streams the rest,
   latches the next line range, and switches the display into the affine
   mode only while that range is on screen. */
void MapAnimation_ApplyAffineFrame(void)
{
    u8 **pointers = gMapAnimationPages;
    u8 *pages = *pointers++ + 0xc80;
    s32 dispcnt = (s16)(*(volatile u16 *)0x04000000 & 0xfff8);
    struct MapAffineWork *work = *(struct MapAffineWork **)pointers++;
    u32 *src;
    u32 *dst;
    u32 mode;
    u32 start;

    {
        volatile u16 *channel = (volatile u16 *)0x040000b0;

        channel[5] &= 0xc5ff;
        channel[5] &= 0x7fff;
        (void)channel[5];
    }
    dst = (u32 *)0x04000020;
    if (pages != NULL) {
        src = (u32 *)(pages + (gFrameCount & 1) * 0x1400);
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst = *src++;
        Dma_Set(src, (void *)0x04000020, 0xa6600008, (volatile u32 *)0x040000b0);
    }
    work->start = work->next_start;
    work->end = work->next_end;
    start = work->start;
    mode = 0;
    if (start < 200) {
        u16 end = work->end;

        mode = (end != 0) << 1;
        if (start <= end) {
            mode = 0;
            if (start == 0)
                mode = 2;
        }
    }
    dispcnt |= mode;
    Io_Set16((u16)dispcnt, (volatile u16 *)0x04000000);
    work->pending = 0;
}
