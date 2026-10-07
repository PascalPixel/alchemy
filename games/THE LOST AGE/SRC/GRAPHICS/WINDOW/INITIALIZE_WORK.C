#include "TYPES.H"
#include "DMA.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"

struct InventoryMenuState {
    u8 unknown_000[28];
    s8 pane_index[2];
    s8 pane_count[2];
    u8 unknown_020[214];
    u8 column_count;
    u8 row_count;
};

void UiWindow_InitializeWork(s32 unused)
{
    struct InventoryMenuState *work = Ram_HeapSlots->menu_runtime;

    Dma_Fill32(0, work, 827 * 4);
    work->pane_index[0] = -1;
    work->pane_count[0] = 1;
    work->pane_count[1] = 1;
    work->column_count = 1;
    work->row_count = 1;
}
