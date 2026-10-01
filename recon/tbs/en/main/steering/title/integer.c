/* MATCHING (2026-10-01): complete ordinary EN compile/link 356/356 bytes,
 * 0 differing bytes including pools. Preserved finite decode/DMA carrier
 * experiment. The selected integer-address form also matches in the complete
 * merged840-byte source module in all six editions; no compiler or output edit.
 */
/* 2026-09-30 (Mercury's helper, stopped at the wind-down): 23 differing
   halfwords, 356 of 356 bytes (the previous draft compiled to 164 at 360).
   2026-10-01 (☀️ matcher 1): the remaining difference is global register
   allocation. The reference copies the decoded buffer's address (r5, the
   constant loaded before the two resource calls) into r6 for the buffer
   pointer and gives the loop counter i r5; here buffer stays in r5 and i
   takes r6, which also swaps the final loop's add to adds r0, r5, r0 (the
   reference has adds r0, r0, r6). The greg dump allocates i before buffer;
   i avoids r5 because buffer prefers it through its copy from the local
   constant register. Unchanged: buffer declared before i, the offset added
   before buffer, an index expression, return 0, gBgScroll[2].y = 0;
   worse: buffer assigned before the decode (24 lines), two loop counters
   (20), an s32 i (20), a separate frames pointer (47). A 300 s permuter
   run (134,586 candidates) found nothing below 165; the listing labels
   this function Func_080f2d54 (pass --symbol). */
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
    /* FAKEMATCH: separate the post-decode buffer carrier from the resource-number carrier. */
    register u8 *buffer __asm__("r6");
    /* FAKEMATCH: keep the decode destination carrier distinct until the buffer copy. */
    register u8 *decode __asm__("r5");
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
    decode = Ram_MapCellBuffer;
    Resource_DecodeType01(Resource_GetTableEntry(resource), decode);
    buffer = decode;
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
        /* FAKEMATCH: the unsigned address sum retains the DMA source operand order. */
        Dma_Set((void *)((((gFrameCount >> 3) & 3) << 10) + (u32)buffer), (void *)0x06004100, 0x840000d0, (volatile u32 *)0x040000d4);
        if (gKeyState & 9)
            break;
        WaitFrames(1);
    }
    return result;
}
