#include "types.h"
#include "dma.h"
#include "flash.h"
#include "ram_buffer.h"
#include "save_state.h"
#include "runtime_interfaces.h"
#include "irq.h"
#include "io_reg.h"

s32 SaveState_LoadExtraSummary(void) {
  struct SaveWorkspace *work;
  struct SaveSummary *summary;
  u32 index;

  work = Ram_HeapSlots->serialWork;
  summary = &work->extra;
  Dma_Fill32(0, summary, sizeof(*summary));
  index = SaveState_FindLatestSlot(3);
  if (index <= 14) {
    ReadFlash(index, 0, (u8 *)summary, sizeof(*summary));
    ReadFlash(index + 1, 0x110, work->extra.unk38, sizeof(work->extra.unk38));
    return 1;
  }
  return 0;
}

void SaveState_ReleaseWorkspace(void) {
  Runtime_SetIrqHandler(5, 0, NULL);
  Runtime_ReleaseHeapBlock(204);
}
