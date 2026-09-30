/* 2026-09-30 (Mercury's helper, stopped at the wind-down): 23 differing
   halfwords, 356 of 356 bytes (the previous draft compiled to 164 at 360).
   The remaining difference was not yet analysed. */
#include "TYPES.H"
#include "SYSTEM.H"
#include "DMA.H"
#include "RESOURCE_IDS.H"
#include "RAM_BUFFER.H"

struct BgScroll {
    s16 x;
    s16 y;
};

extern u8 gOamCopyEnabled;
extern struct BgScroll gBgScroll[4];
extern u32 gFrameCount;
extern volatile u32 gKeyState;

void Scheduler_ResetTaskTable(void);
void Blend_SetDarkenTarget16(s32 frames);
void Blend_SetBrightenTarget0(s32 frames);
void Blend_WaitForTransition(void);
void Bg0_ClearTilemap(void);
void Ui_LoadWindowGraphics(void);
u8 *Resource_GetTableEntry(s32 index);
s32 Resource_DecodeType01(const void *source, void *destination);

/* Title: show the animated splash picture. Its four 1KB tile frames cycle
   every eight frames for up to two seconds, or until A or START. */
s32 Title_ShowAnimatedSplash(void)
{
    s32 result;
    u32 i;
    u8 *buffer;
    s32 resource;

    gOamCopyEnabled = 1;
    result = 0;
    resource = (s32)&ResourceId_CamelotLogo;
    Scheduler_ResetTaskTable();
    Blend_SetDarkenTarget16(1);
    Bg0_ClearTilemap();
    WaitFrames(1);
    *(volatile u16 *)0x0400000c = 0x685;
    *(volatile u16 *)0x04000000 = 0x1440;
    gBgScroll[2].y = result;
    Resource_DecodeType01(Resource_GetTableEntry(resource), Ram_MapCellBuffer);
    buffer = Ram_MapCellBuffer;
    Dma_Set(buffer, (void *)0x05000000, 0x84000070, (volatile u32 *)0x040000d4);
    buffer += 0x1c0;
    Dma_Set(buffer, (void *)0x06003000, 0x84000200, (volatile u32 *)0x040000d4);
    buffer += 0x800;
    Dma_Set(buffer, (void *)0x06004000, 0x84001000, (volatile u32 *)0x040000d4);
    buffer += 0x4000;
    for (i = 0; i < 4; i++) {
        gBgScroll[i].y = 0;
        gBgScroll[i].x = 0;
    }
    Dma_Set(gBgScroll, (void *)0x04000010, 0x84000004, (volatile u32 *)0x040000d4);
    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
    Blend_SetBrightenTarget0(1);
    Blend_WaitForTransition();
    *(volatile u16 *)0x04000000 = 0x1540;
    for (i = 0; i < 120; i++) {
        Dma_Set(buffer + (((gFrameCount >> 3) & 3) << 10), (void *)0x06004100, 0x840000d0, (volatile u32 *)0x040000d4);
        if (gKeyState & 9)
            break;
        WaitFrames(1);
    }
    return result;
}
