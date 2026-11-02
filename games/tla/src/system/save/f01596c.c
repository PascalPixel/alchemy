#include "types.h"
#include "heap_release.h"
#include "dma.h"
#include "flash.h"
#include "irq.h"
#include "system.h"
#include "savedata.h"
#include "save.h"

#define DMA3 ((volatile struct DmaRegisters *)&REG_DMA3SAD)
#define DMA3_BUSY (DMA_ENABLE << 16)

#define START_DMA(source_, destination_, control_)                             \
  Dma_Set((const void *)(source_), (void *)(destination_), (control_), REG_DMA3)

static __inline__ void WaitDma(struct DmaRegisters volatile *channel) {
  while ((channel->control & DMA3_BUSY) != 0) {
  }
}

#define WAIT_DMA() WaitDma(DMA3)

s32 Save_CompareBytes(u8 *left, u8 *right, s32 count);
s32 Save_ReadSector(s32 index);

s32 Func_0801596c(void) {
  struct SaveWorkspace *work;
  struct SaveSectorHeader header;
  struct SaveSectorHeader *slot;
  u32 index;

  work = HeapSlot_AllocEwram(204, sizeof(*work));
  Dma_Fill32(0, work, sizeof(*work));
  SetFlashTimerIntr(2, &gIrqHandlers[5]);

  for (index = 0; index < 8; index++) {
    if ((u16)IdentifyFlash() == 0)
      break;
    wait(1);
  }
  if (index >= 8)
    return 1;

  for (index = 0; index < 15; index += 3) {
    u32 status;
    u16 partId;
    u8 id;

    work->occupied[index] = 0;
    work->partId[index] = 16;
    work->counter[index] = 0;
    status = Save_ReadSector(index);
    START_DMA(work->slot, &header, DMA_COPY_WORDS(4));
    WAIT_DMA();
    slot = &header;

    if (Save_CompareBytes(slot->signature, (u8 *)gSave_Signature, 7) != 0)
      continue;
    work->counter[index] = slot->counter;
    partId = slot->partId;
    id = partId;
    if (id >= 16)
      continue;
    if (status != 0)
      continue;

    work->occupied[index] = 1;
    work->partId[index] = partId;
    if (status < index) {
      do {
        if (work->partId[status] == (u8)partId) {
          if (work->counter[status] < slot->counter)
            work->occupied[status] = 0;
          else
            work->occupied[index] = 0;
        }
        status++;
      } while (status < index);
    }
  }
  return 0;
}
