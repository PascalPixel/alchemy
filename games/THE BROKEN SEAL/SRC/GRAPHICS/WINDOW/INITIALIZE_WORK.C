#include "DMA.H"
#include "INVENTORY_MENU.H"

void UiWindow_InitializeWork(s32 unused)
{
    struct InventoryMenuState *work = gMenuWork;
    volatile u32 zero = 0;
    Dma_Set(&zero, work, 0x8500029c, (volatile u32 *)0x040000d4);
    work->pane_index[0] = -1;
    work->pane_count[0] = 1;
    work->pane_count[1] = 1;
    work->column_count = 1;
    work->row_count = 1;
}
