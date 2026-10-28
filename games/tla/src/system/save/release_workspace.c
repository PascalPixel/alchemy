#include "types.h"
#include "dma.h"
#include "flash.h"
#include "ram_buffer.h"
#include "save.h"
#include "runtime_interfaces.h"
#include "irq.h"
#include "io_reg.h"

s32 Save_LoadExtraSummary(void) {
  struct SaveWorkspace *work;
  struct SaveSummary *summary;
  u32 index;

  work = Ram_HeapSlots->serialWork;
  summary = &work->extra;
  Dma_Fill32(0, summary, sizeof(*summary));
  index = Save_FindLatestSector(3);
  if (index <= 14) {
    ReadFlash(index, 0, (u8 *)summary, sizeof(*summary));
    ReadFlash(index + 1, 0x110, work->extra.unk38, sizeof(work->extra.unk38));
    return 1;
  }
  return 0;
}

void Save_ReleaseWork(void) {
  Irq_SetHandler(5, 0, NULL);
  HeapSlot_Release(204);
}
