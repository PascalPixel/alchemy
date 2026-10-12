#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "bg_scroll.h"
#include "btldisp.h"

extern const u8 Data_0812cc74[];

void Func_08125c5c(void)
{
    u32 index = *(s32 *)Ram_HeapSlots->unknown_84[9] - 1;

    if (index <= 31)
        Dma_Set(Data_0812cc74 + (index >> 2) * 32, (void *)0x06005000, 0x84000008, REG_DMA3);
}

void BattlePres_AdvanceTransitionTimer(void)
{
    s32 scroll;
    struct BattleCamera *camera;
    u32 *timer;
    u32 frame;
    u32 next;

    timer = (u32 *)Ram_HeapSlots->unknown_84[9];
    frame = *timer;
    camera = (struct BattleCamera *)Ram_HeapSlots->camera_work;
    scroll = 0x34 - frame;
    if (scroll > 0x20)
        scroll = 0x20;
    if (scroll < 0)
        scroll = 0;
    gBgScroll[0].y = scroll;
    if (frame <= 0x50U)
        camera->yaw = 45 * frame * 8 + 0xaf80;
    next = (*timer = *timer + 1);
    if (next <= 0x50U) {
        BattlePres_SetupTransitionScene(0, 0, 0, 0xb4 - next);
        return;
    }
    BattlePres_SetupTransitionScene(0, 0, 0, 0x64);
}
