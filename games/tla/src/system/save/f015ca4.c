#include "types.h"
#include "ram_buffer.h"
#include "save_state.h"
#include "dma.h"
#include "flash.h"

#define DMA3 ((volatile struct DmaRegisters *)0x040000d4)
#define DMA3_BUSY 0x80000000

#define START_DMA(source_, destination_, control_)                             \
  Dma_Set((const void *)(source_), (void *)(destination_), (control_),         \
          (volatile u32 *)0x040000d4)

static __inline__ void WaitDma(struct DmaRegisters volatile *channel) {
  while ((channel->control & DMA3_BUSY) != 0) {
  }
}

#define WAIT_DMA() WaitDma(DMA3)

extern u16 (*gFlash_Handler0)(u16 sector, u8 *source);
extern const u8 gSave_Signature[];
extern const u8 gSave_HeaderTemplate[];

u32 Random16(void);
s32 SaveState_CompareBytes(u8 *left, u8 *right, s32 count);
s32 SaveState_ChecksumWorkspace(void);
s32 SaveState_ReadSlotAndCheckChecksum(s32 index);
u32 SaveState_WriteWorkspaceSlot(s32 sector);
s32 SaveState_InvalidateSlot(s32 index);

u32 SaveState_SelectWriteSlot(s32 recordId);
u32 SaveState_FindLatestSequence(s32 recordId);

s32 Func_08015ca4(s32 recordId, void *source) {
  struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
  struct SaveSlotHeader header;
  volatile u32 zero;
  u32 current;
  u32 slot;

  zero = 0;
  START_DMA(&zero, work->slot, 0x85000c00);
  WAIT_DMA();
  current = SaveState_FindLatestSlot(recordId);
  slot = SaveState_SelectWriteSlot(recordId);
  if (slot > 14)
    return 1;
  START_DMA(source, work->slot + 16, 0x84000bfc);
  WAIT_DMA();
  START_DMA(gSave_HeaderTemplate, &header, 0x84000002);
  WAIT_DMA();
  header.recordId = recordId;
  header.checksum = SaveState_ChecksumWorkspace();
  header.sequence = SaveState_FindLatestSequence(recordId) + 1;
  START_DMA(&header, work->slot, 0x84000004);
  WAIT_DMA();
  work->sectorCount = 0;
  work->sectorTotal = 3;
  if (current <= 14)
    work->sectorTotal = 6;
  if (SaveState_WriteWorkspaceSlot(slot) != 0)
    return 2;
  if (current <= 14 && SaveState_InvalidateSlot(current) != 0)
    return 3;
  if (header.sequence > 0xfde8) {
    header.sequence = 1;
    START_DMA(&header, work->slot, 0x84000004);
    WAIT_DMA();
    if (SaveState_WriteWorkspaceSlot(current) != 0)
      return 4;
    if (SaveState_InvalidateSlot(slot) != 0)
      return 5;
    slot = current;
  }
  work->occupied[slot] = 1;
  work->recordId[slot] = recordId;
  work->sequence[slot] = header.sequence;
  return 0;
}
