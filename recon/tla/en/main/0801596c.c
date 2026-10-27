// Draft, differ 111 (118 before statements reordered 2026-10-26) of 164: allocates the save workspace, identifies the
// flash chip and scans the sectors three at a time, as ☀️'s
// SaveState_InitializeWorkspace (games/tbs/src/runtime/runtime.c) over 15
// sectors. Same instructions in the same shape; what is left is register
// choice: the native keeps the workspace in fp (here a stack slot), the
// occupied pointer in r7 and the record id byte addressed as a stack offset
// plus base, where this draft makes each its own induction register.
#include "types.h"
#include "mem.h"
#include "dma.h"
#include "flash.h"
#include "irq.h"
#include "system.h"
#include "savedata.h"
#include "save_state.h"

#define START_DMA(source_, destination_, control_)                             \
  Dma_Set((const void *)(source_), (void *)(destination_), (control_),         \
          (volatile u32 *)0x040000d4)

static __inline__ void WaitDma(struct DmaRegisters volatile *channel) {
  while ((channel->control & 0x80000000) != 0) {
  }
}

s32 SaveState_CompareBytes(u8 *left, u8 *right, s32 count);
s32 SaveState_ReadSlotAndCheckChecksum(s32 index);

s32 Func_0801596c(void) {
  struct SaveWorkspace *work;
  struct SaveSlotHeader header;
  struct SaveSlotHeader *slot;
  u32 index;

  work = Runtime_AllocateBlock(204, sizeof(*work));
  Dma_Fill32(0, work, sizeof(*work));
  SetFlashTimerIntr(2, &gIrqHandlers[5]);

  for (index = 0; index < 8; index++) {
    if ((u16)IdentifyFlash() == 0)
      break;
    WaitTicks(1);
  }
  if (index >= 8)
    return 1;

  for (index = 0; index < 15; index += 3) {
    u32 status;
    u32 record;
    u8 id;

    work->occupied[index] = 0;
    work->recordId[index] = 16;
    work->sequence[index] = 0;
    status = SaveState_ReadSlotAndCheckChecksum(index);
    START_DMA(work->slot, &header, 0x84000004);
    WaitDma((volatile struct DmaRegisters *)0x040000d4);
    slot = &header;

    if (SaveState_CompareBytes(slot->signature, (u8 *)gSave_Signature, 7) != 0)
      continue;
    record = slot->recordId;
    id = record;
    work->sequence[index] = slot->sequence;
    if (id >= 16)
      continue;
    if (status != 0)
      continue;

    work->occupied[index] = 1;
    work->recordId[index] = record;
    if (status < index) {
      do {
        if (work->recordId[status] == (u8)record) {
          if (work->sequence[status] < slot->sequence)
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
