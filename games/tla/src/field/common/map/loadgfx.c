#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "map.h"
#include "resource.h"
#include "system.h"
#include "frame.h"
#include "callback_scheduler.h"

void MapAnimation_PresentFrame(void)
{
    s32 control;
    volatile u32 *dma;

    control = 0xa80b;
    REG_BG3CNT = control;
    control = 0xaa0f;
    REG_BG2CNT = control;
    control = 0x682;
    REG_BG1CNT = control;
    dma = REG_DMA3;
    /* FAKEMATCH: ⚓️ addresses the DMA channel before loading the transfer operands */
    __asm__ volatile("" : "+l"(dma));
    Dma_Set(Ram_MapCellBuffer, (void *)0x06006000, 0x84002800, dma);
}

extern void (*Data_030011f8)(void);
extern u8 Data_02038000[];
extern u8 Data_0203a000[];
extern u8 Data_0201c000[];
void MapAnimation_Update(void);
void MapAnimation_PresentFrame(void);
void Func_0802dd08(void *source, void *destination);

void MapAnimation_Start(void)
{
    u8 *pages = Ram_HeapSlots->unknown_1c[0];
    struct MapViewWork *work = (struct MapViewWork *)Ram_HeapSlots->map_work;
    u32 *resources;
    s32 one;
    s8 *active;

    active = &work->animation_active;
    /* FAKEMATCH: ⚓️ addresses the active flag before it sets the shared one */
    __asm__ volatile("" : "+l"(active));
    one = 1;
    *active = one;
    Scheduler_EnableCallbacks(MapAnimation_Update);
    work->frame_pending = one;
    resources = work->resources;
    Resource_DecodeType01(Resource_GetTableEntry(resources[3]), Data_02038000);
    if (work->extra_tiles != 0)
        Resource_DecodeType01(Resource_GetTableEntry(resources[4]), Data_0203a000);
    work->extra_tiles = 0;
    Dma_Set((void *)0x06004000, Data_0201c000, 0x84000800, REG_DMA3);
    WaitFrames(1);
    Func_0802dd08(pages + (gFrameCount & one) * 0x1400 + 0xc80, Ram_MapCellBuffer);
    work->window_top = 200;
    work->window_bottom = 255;
    Data_030011f8 = MapAnimation_PresentFrame;
    work->frame_shown = one;
}

extern u8 Data_02038000[];

void Map_ShowBg1FromBuffer(void)
{
    s32 control;
    volatile u32 *dma;

    control = 0xa80a;
    REG_BG3CNT = control;
    control = 0xaa0e;
    REG_BG2CNT = control;
    control = 0x501;
    REG_BG1CNT = control;
    dma = REG_DMA3;
    /* FAKEMATCH: ⚓️ addresses the DMA channel before loading the transfer operands */
    __asm__ volatile("" : "+l"(dma));
    Dma_Set(Data_02038000, (void *)0x06008000, 0x84002000, dma);
}
