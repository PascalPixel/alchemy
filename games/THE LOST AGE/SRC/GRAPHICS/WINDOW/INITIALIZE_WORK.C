#include "TYPES.H"
#include "DMA.H"
#include "RAM_BUFFER.H"

/* The parts of ⚓️'s item and Psynergy menu work this reset sets; the work
   is 0x33b words, larger than ☀️'s. */
struct InventoryMenuState {
    u8 unknown_000[0x1c];
    s8 pane_index[2];
    s8 pane_count[2];
    u8 unknown_020[0xd6];
    u8 column_count;
    u8 row_count;
};

/* ☀️'s reset: clear the work, no pane selected, one pane of each kind and
   a one-by-one grid. */
void UiWindow_InitializeWork(s32 unused)
{
    struct InventoryMenuState *work = Ram_HeapSlots->menu_runtime;
    volatile u32 zero = 0;
    Dma_Set(&zero, work, 0x8500033b, (volatile u32 *)0x040000d4);
    work->pane_index[0] = -1;
    work->pane_count[0] = 1;
    work->pane_count[1] = 1;
    work->column_count = 1;
    work->row_count = 1;
}
