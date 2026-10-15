#include "types.h"
#include "dma.h"
#include "ram_buffer.h"
#include "io_reg.h"

struct InventoryMenuState {
  u8 unk000[0x1c];
  s8 paneIndex[2];
  s8 paneCount[2];
  u8 unk020[0xd6];
  u8 columnCount;
  u8 rowCount;
};

void UiWindow_InitializeWork(s32 unused) {
  struct InventoryMenuState *work = Ram_HeapSlots->menuRuntime;

  Dma_Fill32(0, work, 0x33b * 4);
  work->paneIndex[0] = -1;
  work->paneCount[0] = 1;
  work->paneCount[1] = 1;
  work->columnCount = 1;
  work->rowCount = 1;
}
