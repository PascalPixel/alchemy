/* Pin-free trial (tla-en): listing 141 instructions, draft 138, common 106, differ 67.
 * Earlier register-allocation notes describe the prior pinned form.
 */
#include "resource.h"
#include "types.h"
#include "io_reg.h"
#include "callback_scheduler.h"
#include "system.h"
#include "dma.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"

struct BgScroll { u16 x, y; };

extern struct BgScroll gBgScroll[];
extern char ResourceId_CamelotLogo;

void Blend_SetDarkenTarget16(s32 frames);
void Blend_SetBrightenTarget0(s32 frames);
void Blend_WaitForTransition(void);
void Bg0_ClearTilemap(void);
void Ui_LoadWindowGraphics(void);

s32 Title_ShowAnimatedSplash(void)
{
    u32 i;
    /* FAKEMATCH: separate the post-decode buffer carrier from the resource-number carrier. */
    u8 *buffer;
    /* FAKEMATCH: keep the decode destination carrier distinct until the buffer copy. */
    u8 *decode;
    s32 resource;

    gRenderOamEnabled = 1;
    resource = (s32)&ResourceId_CamelotLogo;
    Scheduler_ResetTaskTable();
    Blend_SetDarkenTarget16(1);
    Bg0_ClearTilemap();
    WaitFrames(1);
    REG_BG2CNT = 0x685;
    REG_DISPCNT = 0x1440;
    gBgScroll[2].y = 0;
    decode = Ram_MapCellBuffer;
    Resource_DecodeType01(Resource_GetTableEntry(resource), decode);
    buffer = decode;
    Dma_Set(buffer, (void *)BG_PLTT, DMA_ENABLE32 | DMA_32BIT | 0x1c0 / 4, REG_DMA3);
    buffer += 0x1c0;
    Dma_Set(buffer, (void *)0x06003000, DMA_ENABLE32 | DMA_32BIT | 0x800 / 4, REG_DMA3);
    buffer += 0x800;
    Dma_Set(buffer, BG_CHAR_BLOCK(1), DMA_ENABLE32 | DMA_32BIT | 0x4000 / 4, REG_DMA3);
    buffer += 0x4000;
    for (i = 0; i < 4; i++) {
        gBgScroll[i].y = 0;
        gBgScroll[i].x = 0;
    }
    Dma_Set(gBgScroll, REG_BG0HOFS, DMA_ENABLE32 | DMA_32BIT | 4, REG_DMA3);
    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
    Blend_SetBrightenTarget0(1);
    Blend_WaitForTransition();
    REG_DISPCNT = 0x1540;
    for (i = 0; i < 120; i++) {
        /* FAKEMATCH: the unsigned address sum retains the DMA source operand order. */
        Dma_Set((void *)((((gFrameCount >> 3) & 3) << 10) + (u32)buffer), BG_CHAR_BLOCK(1) + 0x100, DMA_ENABLE32 | DMA_32BIT | 0x340 / 4, REG_DMA3);
        if (gInput.pressed & (KEY_A | KEY_START))
            break;
        WaitFrames(1);
    }
    return 0;
}

void Title_NoOpPalette(void)
{
}
