#include "dma.h"
#include "inventory_menu.h"
#include "io_reg.h"

void UiWindow_InitializeWork(s32 unused)
{
    struct InventoryMenuState *work = gMenuWork;
    volatile u32 zero = 0;
    Dma_Set(&zero, work, 0x8500029c, REG_DMA3);
    work->pane_index[0] = -1;
    work->pane_count[0] = 1;
    work->pane_count[1] = 1;
    work->column_count = 1;
    work->row_count = 1;
}
