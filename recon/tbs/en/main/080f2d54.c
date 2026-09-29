/* 2026-09-29: eight minutes of permutation (alchemy permute --symbol
 * Unnamed_080f2d54): 910 -> 765 by setting the zero before the
 * display-register writes. Remaining: the extra saved r7 and the
 * buffer/cursor ownership described below, plus the resource number 0x19,
 * still a Value_ symbol (a plain 0x19 gives 980). A second 8-minute run
 * from 765 with another seed found nothing lower. */
/* Draft, not exact (2026-09-24): 33 differing halfwords, 356 of 356 bytes.
   Control flow, pools and stores match. Residual is allocation: the
   reference keeps the resource id and then the decode buffer in r6 (a
   copy of the 0x02010000 destination in r5) and both loop counters in r5;
   here the buffer takes r5, the counters r6 and the zero r7. Declaration
   orders, shared or separate counters and five buffer spellings did not
   move it; the zero in the offset loop is what brings it from 68 to 33.
   2026-09-27 bounded scroll-interface trial: the exact TITLE/BG_SETUP.C
   reset loop, scoped in an inline ResetScroll helper with its own counter,
   removes the extra saved r7 but gives 356 bytes / 69 halfwords / 39 edits.
   Keeping the reset counter in the caller instead gives 356 / 68 / 38.
   Both still coalesce dst and buffer into r5, omit the reference's r6 copy,
   and move the first zero/store; the helper also assigns its counter r2.
   Fresh retained baseline is 356 / 33 / 27. Independent reset-zero lifetime
   alone does not recover the resource/cursor ownership; no adoption.
   2026-09-29 stock agscc, the decode buffer through its linker-placed
   name Ram_MapCellBuffer (literal RAM addresses are no longer allowed):
   Title_ShowAnimatedSplash, 356 of 356 bytes, 49 differing lines, 52
   aligned edits. Passing Ram_MapCellBuffer to the decoder and then copying
   it keeps the reference's buffer copy (r5 to the cursor) but loads the
   symbol after the entry call; assigning it to a variable before the call
   hoists the load like the reference but GCC then propagates the symbol
   and drops the copy (65 lines). A constant base keeps both, a symbol
   neither. Declaration order does not move either form. Needs a form of
   the named buffer that is loaded before the call and still copied. */
#include "TYPES.H"
#include "SYSTEM.H"
#include "DMA.H"
#include "RESOURCE_IDS.H"
#include "RAM_BUFFER.H"

struct BgScroll {
    s16 x;
    s16 y;
};

extern u8 Data_03001d18;
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
    s32 resource;
    u32 i;
    u8 *buffer;
    struct BgScroll *scroll;
    s32 zero;

    Data_03001d18 = 1;
    resource = (s32)&ResourceId_CamelotLogo;
    Scheduler_ResetTaskTable();
    Blend_SetDarkenTarget16(1);
    Bg0_ClearTilemap();
    WaitFrames(1);
    zero = 0;
    *(volatile u16 *)0x0400000c = 0x685;
    *(volatile u16 *)0x04000000 = 0x1440;
    scroll = gBgScroll;
    scroll[2].y = zero;
    Resource_DecodeType01(Resource_GetTableEntry(resource), Ram_MapCellBuffer);
    buffer = Ram_MapCellBuffer;
    Dma_Set(buffer, (void *)0x05000000, 0x84000070, (volatile u32 *)0x040000d4);
    buffer += 0x1c0;
    Dma_Set(buffer, (void *)0x06003000, 0x84000200, (volatile u32 *)0x040000d4);
    buffer += 0x800;
    Dma_Set(buffer, (void *)0x06004000, 0x84001000, (volatile u32 *)0x040000d4);
    buffer += 0x4000;
    for (i = 0; i < 4; i++, scroll++)
        scroll->x = scroll->y = zero;
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
    return 0;
}
