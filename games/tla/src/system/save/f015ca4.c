#include "types.h"
#include "ram_buffer.h"
#include "save.h"
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
s32 Save_CompareBytes(u8 *left, u8 *right, s32 count);
s32 Save_ChecksumSector(void);
s32 Save_ReadSector(s32 index);
u32 Save_WriteSector(s32 sector);
s32 Save_InvalidateSector(s32 index);

u32 Save_SelectWriteSector(s32 recordId);
u32 Save_GetLatestCounter(s32 recordId);

s32 Func_08015ca4(s32 recordId, void *source) {
  struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
  struct SaveSlotHeader header;
  volatile u32 zero;
  u32 current;
  u32 slot;

  zero = 0;
  START_DMA(&zero, work->slot, 0x85000c00);
  WAIT_DMA();
  current = Save_FindLatestSector(recordId);
  slot = Save_SelectWriteSector(recordId);
  if (slot > 14)
    return 1;
  START_DMA(source, work->slot + 16, 0x84000bfc);
  WAIT_DMA();
  START_DMA(gSave_HeaderTemplate, &header, 0x84000002);
  WAIT_DMA();
  header.recordId = recordId;
  header.checksum = Save_ChecksumSector();
  header.counter = Save_GetLatestCounter(recordId) + 1;
  START_DMA(&header, work->slot, 0x84000004);
  WAIT_DMA();
  work->sectorCount = 0;
  work->sectorTotal = 3;
  if (current <= 14)
    work->sectorTotal = 6;
  if (Save_WriteSector(slot) != 0)
    return 2;
  if (current <= 14 && Save_InvalidateSector(current) != 0)
    return 3;
  if (header.counter > 0xfde8) {
    header.counter = 1;
    START_DMA(&header, work->slot, 0x84000004);
    WAIT_DMA();
    if (Save_WriteSector(current) != 0)
      return 4;
    if (Save_InvalidateSector(slot) != 0)
      return 5;
    slot = current;
  }
  work->occupied[slot] = 1;
  work->recordId[slot] = recordId;
  work->counter[slot] = header.counter;
  return 0;
}
