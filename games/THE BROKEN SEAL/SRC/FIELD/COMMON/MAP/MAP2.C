#include "DMA.H"
#include "MAP.H"

extern u8 gMapCellBuffer[];

extern u32 gFrameCount;
extern u8 Data_03001cfc[];
extern u8 gDecodeBuffer[];
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

/* Display hook installed by MapAnimation_Start: selects the animation
   layout for BG1 and copies the decoded frame into character VRAM. */
void MapAnimation_PresentFrame(void)
{
    s32 control = 0x682;

    /* FAKEMATCH: the do-while keeps the BG1CNT store ahead of the DMA
       source and destination loads. */
    do {
        *(volatile u16 *)0x0400000a = control;
    } while (0);
    Dma_Set((const void *)gMapCellBuffer, (void *)0x06006a00, 0x84002580, (volatile u32 *)0x040000d4);
}

/* Starts the map animation task, saves the second character block, and
   decodes this frame's animation page into the buffer. */
void MapAnimation_Start(void)
{
    /* FAKEMATCH: the map work pointer, gMapWork, is read as the word after
       the pages through one base; the reference loads one address for both. */
    u8 **pointers = &gMapAnimationPages;
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

/* Points BG1 at character block 2 and copies the 32 KB of buffered
   characters into it. */
void Map_ShowBg1FromBuffer(void)
{
    u32 mode = 0x501;
    u16 *bg1cnt = (u16 *)0x0400000a;

    do { *bg1cnt = mode; } while (0); /* FAKEMATCH: the wrap orders the store before the DMA operands. */
    Dma_Set((const void *)gBgTileBuffer, (void *)0x06008000, 0x84002000,
            (volatile u32 *)((u8 *)bg1cnt + 0xca));
}
