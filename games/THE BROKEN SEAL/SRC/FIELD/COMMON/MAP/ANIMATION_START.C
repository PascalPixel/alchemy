#include "DMA.H"
extern u32 gFrameCount;
extern u8 Data_03001cfc[];
extern u8 gDecodeBuffer[];
extern u8 gMapCellBuffer[];
void MapAnimation_Update(void);
void MapAnimation_PresentFrame(void);

struct MapAnimationWork {
    u8 unknown_000[0xfc];
    u8 active;
    u8 unknown_0fd[3];
    u16 timer;
    u16 limit;
};


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
    Scheduler_EnableCallbacks(MapAnimation_Update);
    Dma_Set((const void *)0x06004000, (void *)gDecodeBuffer, 0x84000800, (volatile u32 *)0x040000d4);
    WaitFrames(1);
    Resource_RunCopiedDecoder(pages + (*(u32 *)&gFrameCount & one) * 0x1400 + 0xc80, (void *)gMapCellBuffer);
    work->timer = 200;
    work->limit = 255;
    *(u32 *)Data_03001cfc = (u32)MapAnimation_PresentFrame;
}
