#include "DMA.H"

struct MapAnimationWork {
    u8 unknown_000[0xfc];
    u8 active;
    u8 unknown_0fd[3];
    u16 timer;
    u16 limit;
};

#define MapAnimation_Start Func_08011590

s32 Scheduler_EnableCallbacks(void (*callback)(void));
void WaitFrames(s32 frames);
void Resource_RunCopiedDecoder(void *source, void *destination);

/* Starts the map animation task, saves the second character block, and
   decodes this frame's animation page into the buffer. */
void MapAnimation_Start(void)
{
    u8 **pointers = (u8 **)0x03001e6c;
    u8 *pages = *pointers++;
    struct MapAnimationWork *work = *(struct MapAnimationWork **)pointers;
    s32 one = 1;

    work->active = one;
    Scheduler_EnableCallbacks((void (*)(void))0x0801179d);
    Dma_Set((const void *)0x06004000, (void *)0x0201c000, 0x84000800, (volatile u32 *)0x040000d4);
    WaitFrames(1);
    Resource_RunCopiedDecoder(pages + (*(u32 *)0x03001e40 & one) * 0x1400 + 0xc80, (void *)0x02010000);
    work->timer = 200;
    work->limit = 255;
    *(u32 *)0x03001cfc = 0x08011569;
}
